#ifndef MODBUSCOMM_H
#define MODBUSCOMM_H

#include <QObject>
#include <QString>
#include <QHash>
#include <QList>
#include <QPair>
#include <QVector>

// 前向声明（避免在头文件中包含大的头文件，加快编译）
class QModbusTcpClient;
class QModbusReply;
class QTimer;

// 类型别名
using RegValueMap = QHash<quint16, quint16>;   // 寄存器地址 -> 原始值
using ReadGroupList = QList<QPair<quint16, quint16>>; // 读取段列表（起始地址, 数量）

class ModbusComm : public QObject {
    Q_OBJECT
public:
    explicit ModbusComm(QObject* parent = nullptr);
    ~ModbusComm();

    //  连接管理
    // 用户主动连接：设置"用户意图为连接"，断线后自动重连
    void connectTo(const QString& ip, quint16 port, int slaveId);
    // 用户主动断开：清除"用户意图"，断线后不重连
    void disconnectFromHost();
    // 检查是否已连接
    bool isConnected() const;

    //  读取配置
    // 设置聚合后的读取段
    void setReadGroups(const ReadGroupList& groups);
    // 设置轮询间隔（毫秒）
    void setPollInterval(int ms);

    //  写入接口
    // 写入单个寄存器
    void writeRegister(quint16 reg, quint16 value);
    // 批量写入寄存器
    void writeRegisters(quint16 startReg, const QVector<quint16>& values);

signals:
    // 连接状态变化
    void connectionStateChanged(bool connected, const QString& msg);
    // 通讯错误
    void errorOccurred(const QString& err);
    // 收到一轮原始数据
    void rawDataReceived(RegValueMap regValues);
    // 写入完成
    void writeFinished(quint16 reg, bool ok, const QString& err);

private slots:
    // 连接状态变化处理
    void onStateChanged();
    // 轮询定时器超时
    void onPollTimeout();
    // 重连定时器超时
    void onReconnectTimeout();

private:
    //  核心成员
    QModbusTcpClient* _client = nullptr;   // Modbus TCP 客户端
    QTimer* _pollTimer = nullptr;          // 轮询定时器
    QTimer* _reconnectTimer = nullptr;     // 重连定时器

    //  连接参数
    QString _ip;
    quint16 _port = 502;
    int _slaveId = 1;
    int _pollInterval = 1000; // 轮询间隔（毫秒）

    //  状态标志
    bool _userRequestedConnect = false; // true=希望保持连接（断线重连）；false=已主动断开（不重连）

    //  数据缓存
    ReadGroupList _groups;    // 聚合后的读取段
    RegValueMap _accumulated; // 本轮累积的原始数据
    int _pendingReplies = 0;  // 等待中的请求数量

    //  内部辅助函数
    void startPollTimer();
    void stopPollTimer();
    void scheduleReconnect();   // 启动重连定时器
    void doConnect();           // 实际执行连接（首次和重连共用）
    void sendOneReadRequest(quint16 startReg, quint16 count);
    void handleReadReply(QModbusReply* reply);
    void handleWriteReply(QModbusReply* reply, quint16 reg);
};

#endif // MODBUSCOMM_H