#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "config/configeditdialog.h"
#include "mainwindow/connectdialog.h"
#include "curve/yrangedialog.h"

#include <QLabel>
#include <QStatusBar>
#include <QDebug>
#include <QSettings>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSplitter>
#include <algorithm>

// 主窗口构造函数：初始化整个软件界面 + 所有模块连接
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 状态栏连接标签
    _lblConn = new QLabel(QString::fromUtf8("未连接"), this);
    statusBar()->addWidget(_lblConn);

    // 菜单动作绑定
    connect(ui->actionLoadConfig, &QAction::triggered, this, &MainWindow::onActionLoadConfig);
    connect(ui->actionSaveConfig, &QAction::triggered, this, &MainWindow::onActionSaveConfig);
    connect(ui->actionEditConfig, &QAction::triggered, this, &MainWindow::onActionEditConfig);
    connect(ui->actionSelectSaveDir, &QAction::triggered, this, &MainWindow::onActionSelectSaveDir);
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::onActionConnect);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::onActionDisconnect);

    // 通讯状态、错误
    connect(&_comm, &ModbusComm::connectionStateChanged, this, &MainWindow::onConnectionChanged);
    connect(&_comm, &ModbusComm::errorOccurred, this, &MainWindow::onCommError);

    // 调试打印原始数据
    connect(&_comm, &ModbusComm::rawDataReceived, this, [](RegValueMap data){
        qDebug() << "----- 收到一轮数据 -----";
        auto keys = data.keys();
        std::sort(keys.begin(), keys.end());
        for (auto k : keys)
            qDebug() << "  reg" << k << "=" << data.value(k);
    });

    // 主界面布局搭建
    setupLayout();

    // 数据解析器
    _parser = new DataParser(&_cfg, this);
    connect(&_comm, &ModbusComm::rawDataReceived, _parser, &DataParser::onRawDataReceived);
    connect(_parser, &DataParser::pointsUpdated, _curveView, &CurveView::onPointsReceived);

    // CSV 日志
    _logger.setConfigManager(&_cfg);
    connect(_parser, &DataParser::pointsUpdated, &_logger, &CsvLogger::onPointsUpdated);
    connect(&_logger, &CsvLogger::errorOccurred, this, &MainWindow::onCommError);

    // 控制面板：写入寄存器
    _control->setConfigManager(&_cfg);
    connect(_control, &ControlPanel::writeRequested, this,
            [this](quint16 reg, quint16 value){
                _comm.writeRegister(reg, value);
            });
    connect(&_comm, &ModbusComm::writeFinished, _control, &ControlPanel::onWriteFinished);

    // 曲线选择器
    _selector->setConfigManager(&_cfg);
    connect(_selector, &CurveSelector::visibleTagsChanged,
            _curveView, &CurveView::setVisibleTags);

    // 视图菜单
    connect(ui->actionSetYRange, &QAction::triggered, this, &MainWindow::onActionSetYRange);
    connect(ui->actionBoxZoom, &QAction::triggered, this, &MainWindow::onActionBoxZoom);
    connect(ui->actionResetView, &QAction::triggered, this, &MainWindow::onActionResetView);

    // 加载配置与窗口状态
    loadSettings();
    tryAutoLoadConfig();
    onConfigChanged();

    // 设置CSV保存目录
    if (!_lastSaveDir.isEmpty())
        _logger.setSaveDir(_lastSaveDir);
}

// 析构函数：释放 UI
MainWindow::~MainWindow()
{
    delete ui;
}

// 搭建主窗口布局：上下结构 + 左右分割
void MainWindow::setupLayout()
{
    _control = new ControlPanel(this);
    _selector = new CurveSelector(this);
    _curveView = new CurveView(this);

    // 上部分左右分割
    _topSplit = new QSplitter(Qt::Horizontal, this);
    _topSplit->addWidget(_control);
    _topSplit->addWidget(_selector);
    _topSplit->setStretchFactor(0, 1);
    _topSplit->setStretchFactor(1, 1);

    // 整体上下分割
    _mainSplit = new QSplitter(Qt::Vertical, this);
    _mainSplit->addWidget(_topSplit);
    _mainSplit->addWidget(_curveView);
    _mainSplit->setStretchFactor(0, 0);
    _mainSplit->setStretchFactor(1, 1);

    _mainSplit->setSizes({200, 600});
    setCentralWidget(_mainSplit);
}

// 配置变化时统一刷新所有 UI 模块
void MainWindow::onConfigChanged()
{
    _control->refresh();
    _selector->refresh();
    _curveView->resetGraphs();
}

// 窗口关闭事件：保存设置
void MainWindow::closeEvent(QCloseEvent* event)
{
    saveSettings();
    QMainWindow::closeEvent(event);
}

// 加载保存的设置：窗口大小、分割条、上次路径、连接参数等
void MainWindow::loadSettings()
{
    QSettings s;
    _lastConfigPath = s.value("config/lastPath").toString();
    _lastSaveDir = s.value("csv/saveDir").toString();
    _lastIp = s.value("modbus/lastIp", "127.0.0.1").toString();
    _lastPort = static_cast<quint16>(s.value("modbus/lastPort", 502).toUInt());
    _lastSlaveId = s.value("modbus/lastSlaveId", 1).toInt();

    // 恢复窗口
    QByteArray geo = s.value("ui/geometry").toByteArray();
    if (!geo.isEmpty())
        restoreGeometry(geo);

    // 恢复分割器状态
    QByteArray mainSplitState = s.value("ui/mainSplit").toByteArray();
    if (!mainSplitState.isEmpty())
        _mainSplit->restoreState(mainSplitState);

    QByteArray topSplitState = s.value("ui/topSplit").toByteArray();
    if (!topSplitState.isEmpty())
        _topSplit->restoreState(topSplitState);
}

// 保存当前设置到注册表
void MainWindow::saveSettings()
{
    QSettings s;
    s.setValue("config/lastPath", _lastConfigPath);
    s.setValue("csv/saveDir", _lastSaveDir);
    s.setValue("modbus/lastIp", _lastIp);
    s.setValue("modbus/lastPort", _lastPort);
    s.setValue("modbus/lastSlaveId", _lastSlaveId);
    s.setValue("ui/geometry", saveGeometry());
    s.setValue("ui/mainSplit", _mainSplit->saveState());
    s.setValue("ui/topSplit", _topSplit->saveState());
}

// 程序启动时自动加载上次的配置文件
void MainWindow::tryAutoLoadConfig()
{
    if (_lastConfigPath.isEmpty())
        return;

    QFileInfo fi(_lastConfigPath);
    if (!fi.exists() || !fi.isReadable())
        return;

    QString err = _cfg.loadFromCsv(_lastConfigPath);
    if (err.isEmpty())
        statusBar()->showMessage(QString::fromUtf8("已加载：") + _lastConfigPath, 3000);
    else
        statusBar()->showMessage(QString::fromUtf8("启动加载失败：") + err, 5000);
}

//文件菜单
// 加载配置文件
void MainWindow::onActionLoadConfig()
{
    QString startDir;
    if (!_lastConfigPath.isEmpty())
        startDir = QFileInfo(_lastConfigPath).absolutePath();

    QString path = QFileDialog::getOpenFileName(
        this, QString::fromUtf8("加载配置文件"), startDir,
        QString::fromUtf8("CSV 文件 (*.csv);;所有文件 (*.*)"));

    if (path.isEmpty())
        return;

    doLoadConfig(path);
}

// 执行加载配置文件
bool MainWindow::doLoadConfig(const QString& path)
{
    QString err = _cfg.loadFromCsv(path);
    if (!err.isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("加载失败"), err);
        return false;
    }

    _lastConfigPath = path;
    saveSettings();
    statusBar()->showMessage(QString::fromUtf8("已加载：") + path, 3000);

    onConfigChanged();
    return true;
}

// 保存配置文件
void MainWindow::onActionSaveConfig()
{
    if (_cfg.points().isEmpty()) {
        QMessageBox::information(this, QString::fromUtf8("提示"),
                                 QString::fromUtf8("当前没有点位，无需保存"));
        return;
    }

    QString startPath = _lastConfigPath.isEmpty() ? "config.csv" : _lastConfigPath;
    QString path = QFileDialog::getSaveFileName(
        this, QString::fromUtf8("保存配置文件"), startPath,
        QString::fromUtf8("CSV 文件 (*.csv)"));

    if (path.isEmpty())
        return;

    if (!path.endsWith(".csv", Qt::CaseInsensitive))
        path += ".csv";

    doSaveConfig(path);
}

// 执行保存配置
bool MainWindow::doSaveConfig(const QString& path)
{
    QString err = _cfg.saveToCsv(path);
    if (!err.isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("保存失败"), err);
        return false;
    }

    _lastConfigPath = path;
    saveSettings();
    statusBar()->showMessage(QString::fromUtf8("已保存：") + path, 3000);
    return true;
}

// 打开配置编辑对话框（增删改点位）
void MainWindow::onActionEditConfig()
{
    ConfigEditDialog dlg(this);
    dlg.setData(_cfg.points());
    if (dlg.exec() != QDialog::Accepted)
        return;

    _cfg.setPoints(dlg.getData());
    onConfigChanged();
}

// 选择 CSV 保存目录
void MainWindow::onActionSelectSaveDir()
{
    QString dir = QFileDialog::getExistingDirectory(
        this, QString::fromUtf8("选择 CSV 保存目录"), _lastSaveDir);

    if (dir.isEmpty())
        return;

    _lastSaveDir = dir;
    _logger.setSaveDir(dir);
    saveSettings();
    statusBar()->showMessage(QString::fromUtf8("CSV 保存目录：") + dir, 3000);
}

// 通讯菜单
// 打开连接对话框
void MainWindow::onActionConnect()
{
    ConnectDialog dlg(this);
    dlg.setIp(_lastIp);
    dlg.setPort(_lastPort);
    dlg.setSlaveId(_lastSlaveId);

    if (dlg.exec() != QDialog::Accepted)
        return;

    _lastIp = dlg.ip();
    _lastPort = dlg.port();
    _lastSlaveId = dlg.slaveId();
    saveSettings();

    // 开始连接
    _comm.connectTo(_lastIp, _lastPort, _lastSlaveId);

    // 调试输出：聚合后的读取段
    auto groups = _cfg.aggregateReadGroups();
    qDebug() << ">>> 当前点位数:" << _cfg.points().size();
    qDebug() << ">>> 读取段数:" << groups.size();
    for (const auto& g : groups)
        qDebug() << "    段: 起始=" << g.first << " 数量=" << g.second;

    // 设置读取段
    _comm.setReadGroups(groups);
}

// 断开连接
void MainWindow::onActionDisconnect()
{
    _comm.disconnectFromHost();
}

// 通讯状态
// 连接状态变化
void MainWindow::onConnectionChanged(bool connected, const QString& msg)
{
    Q_UNUSED(connected);
    _lblConn->setText(QString("%1  [%2:%3]").arg(msg).arg(_lastIp).arg(_lastPort));
}

// 错误信息显示到状态栏
void MainWindow::onCommError(const QString& err)
{
    statusBar()->showMessage(err, 5000);
}

// 视图菜单
// 设置 Y 轴范围
void MainWindow::onActionSetYRange()
{
    YRangeDialog dlg(this);
    dlg.setRange(_curveView->yMin(), _curveView->yMax());
    dlg.setAutoScale(_curveView->yAutoScale());

    if (dlg.exec() != QDialog::Accepted)
        return;

    if (dlg.autoScale())
        _curveView->setYAutoScale(true);
    else
        _curveView->setYRange(dlg.minValue(), dlg.maxValue());
}

// 开启框选放大模式
void MainWindow::onActionBoxZoom()
{
    _curveView->beginBoxZoom();
    statusBar()->showMessage(
        QString::fromUtf8("框选放大模式：在曲线上拖出矩形（一次性，松开后恢复）"),
        3000);
}

// 重置视图（自动滚动+自动缩放）
void MainWindow::onActionResetView()
{
    _curveView->resetView();
}