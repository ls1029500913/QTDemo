#include "modbuscomm.h"

#include <QModbusTcpClient>
#include <QModbusDataUnit>
#include <QModbusReply>
#include <QTimer>
#include <QVariant>

ModbusComm::ModbusComm(QObject* parent) : QObject(parent)
{
    _client = new QModbusTcpClient(this);
    _client->setTimeout(800);
    _client->setNumberOfRetries(1);

    connect(_client, &QModbusTcpClient::stateChanged, this, &ModbusComm::onStateChanged);
    connect(_client, &QModbusTcpClient::errorOccurred, this, [this](QModbusDevice::Error){
        emit errorOccurred(_client->errorString());
    });

    // 轮询定时器
    _pollTimer = new QTimer(this);
    _pollTimer->setInterval(_pollInterval);
    connect(_pollTimer, &QTimer::timeout, this, &ModbusComm::onPollTimeout);

    // 重连定时器（单次触发）
    _reconnectTimer = new QTimer(this);
    _reconnectTimer->setSingleShot(true);
    _reconnectTimer->setInterval(3000);
    connect(_reconnectTimer, &QTimer::timeout, this, &ModbusComm::onReconnectTimeout);
}

ModbusComm::~ModbusComm()
{
    _client->disconnectDevice();
}

// 用户主动连接
void ModbusComm::connectTo(const QString& ip, quint16 port, int slaveId)
{
    _ip = ip;
    _port = port;
    _slaveId = slaveId;
    _userRequestedConnect = true;
    doConnect();
}

// 用户主动断开
void ModbusComm::disconnectFromHost()
{
    _userRequestedConnect = false;
    _reconnectTimer->stop();
    stopPollTimer();
    _client->disconnectDevice();
}

bool ModbusComm::isConnected() const
{
    return _client->state() == QModbusDevice::ConnectedState;
}

// 实际执行连接
void ModbusComm::doConnect()
{
    if (_client->state() != QModbusDevice::UnconnectedState)
        _client->disconnectDevice();

    _client->setConnectionParameter(QModbusDevice::NetworkAddressParameter, QVariant(_ip));
    _client->setConnectionParameter(QModbusDevice::NetworkPortParameter, QVariant(static_cast<int>(_port)));

    if (!_client->connectDevice()) {
        emit errorOccurred(QString::fromUtf8("连接失败：") + _client->errorString());
        scheduleReconnect();
    }
}

// 配置读取段
void ModbusComm::setReadGroups(const ReadGroupList& groups)
{
    _groups = groups;
    if (isConnected() && !_groups.isEmpty())
        startPollTimer();
    else
        stopPollTimer();
}

void ModbusComm::setPollInterval(int ms)
{
    _pollInterval = ms > 0 ? ms : 1000;
    _pollTimer->setInterval(_pollInterval);
}

// 连接状态变化
void ModbusComm::onStateChanged()
{
    auto state = _client->state();

    if (state == QModbusDevice::ConnectedState) {
        emit connectionStateChanged(true, QString::fromUtf8("已连接"));
        if (!_groups.isEmpty())
            startPollTimer();
    } else if (state == QModbusDevice::UnconnectedState) {
        stopPollTimer();
        emit connectionStateChanged(false, QString::fromUtf8("未连接"));
        if (_userRequestedConnect)
            scheduleReconnect();
    } else if (state == QModbusDevice::ConnectingState) {
        emit connectionStateChanged(false, QString::fromUtf8("连接中..."));
    }
}

// 启动重连
void ModbusComm::scheduleReconnect()
{
    if (!_userRequestedConnect) return;
    if (_reconnectTimer->isActive()) return;

    emit connectionStateChanged(false, QString::fromUtf8("重连中..."));
    _reconnectTimer->start();
}

// 重连超时
void ModbusComm::onReconnectTimeout()
{
    if (!_userRequestedConnect) return;
    if (_client->state() == QModbusDevice::ConnectedState) return;

    doConnect();
}

void ModbusComm::startPollTimer()
{
    if (!_pollTimer->isActive())
        _pollTimer->start();
}

void ModbusComm::stopPollTimer()
{
    if (_pollTimer->isActive())
        _pollTimer->stop();
    _pendingReplies = 0;
    _accumulated.clear();
}

// 轮询超时
void ModbusComm::onPollTimeout()
{
    if (!isConnected()) return;
    if (_groups.isEmpty()) return;
    if (_pendingReplies > 0) return;

    _accumulated.clear();
    _pendingReplies = _groups.size();

    for (const auto& g : _groups)
        sendOneReadRequest(g.first, g.second);
}

// 发送单个读取请求
void ModbusComm::sendOneReadRequest(quint16 startReg, quint16 count)
{
    QModbusDataUnit unit(QModbusDataUnit::HoldingRegisters, startReg, count);

    if (auto* reply = _client->sendReadRequest(unit, _slaveId)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this,
                    [this, reply]{ handleReadReply(reply); });
        } else {
            handleReadReply(reply);
        }
    } else {
        _pendingReplies--;
        emit errorOccurred(_client->errorString());
    }
}

// 处理读取响应
void ModbusComm::handleReadReply(QModbusReply* reply)
{
    if (!reply) return;
    reply->deleteLater();

    if (reply->error() == QModbusDevice::NoError) {
        const auto unit = reply->result();
        for (uint i = 0; i < unit.valueCount(); ++i)
            _accumulated.insert(unit.startAddress() + i, unit.value(i));
    } else {
        emit errorOccurred(QString::fromUtf8("读取错误：") + reply->errorString());
    }

    _pendingReplies--;

    if (_pendingReplies <= 0) {
        _pendingReplies = 0;
        if (!_accumulated.isEmpty())
            emit rawDataReceived(_accumulated);
    }
}

// 写入单个寄存器
void ModbusComm::writeRegister(quint16 reg, quint16 value)
{
    if (!isConnected()) {
        emit writeFinished(reg, false, QString::fromUtf8("未连接"));
        return;
    }

    QModbusDataUnit unit(QModbusDataUnit::HoldingRegisters, reg, 1);
    unit.setValue(0, value);

    if (auto* reply = _client->sendWriteRequest(unit, _slaveId)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this,
                    [this, reply, reg]{ handleWriteReply(reply, reg); });
        } else {
            handleWriteReply(reply, reg);
        }
    } else {
        emit writeFinished(reg, false, _client->errorString());
    }
}

// 批量写入寄存器
void ModbusComm::writeRegisters(quint16 startReg, const QVector<quint16>& values)
{
    if (!isConnected()) {
        emit writeFinished(startReg, false, QString::fromUtf8("未连接"));
        return;
    }
    if (values.isEmpty()) {
        emit writeFinished(startReg, false, QString::fromUtf8("写入数量为 0"));
        return;
    }

    QModbusDataUnit unit(QModbusDataUnit::HoldingRegisters, startReg, values.size());
    for (int i = 0; i < values.size(); ++i)
        unit.setValue(i, values.at(i));

    if (auto* reply = _client->sendWriteRequest(unit, _slaveId)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this,
                    [this, reply, startReg]{ handleWriteReply(reply, startReg); });
        } else {
            handleWriteReply(reply, startReg);
        }
    } else {
        emit writeFinished(startReg, false, _client->errorString());
    }
}

// 处理写入响应
void ModbusComm::handleWriteReply(QModbusReply* reply, quint16 reg)
{
    if (!reply) return;
    reply->deleteLater();

    if (reply->error() == QModbusDevice::NoError)
        emit writeFinished(reg, true, QString());
    else
        emit writeFinished(reg, false, reply->errorString());
}