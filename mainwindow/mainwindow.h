#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//  业务模块头文件
#include "config/configmanager.h"
#include "communication/modbuscomm.h"
#include "data/dataparser.h"
#include "data/csvlogger.h"
//  UI模块头文件
#include "curve/curveview.h"
#include "control/controlpanel.h"
#include "control/curveselector.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//  Qt前向声明
class QLabel;
class QSplitter;

// 主窗口
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // 窗口关闭前保存设置
    void closeEvent(QCloseEvent* event) override;

private slots:
    //  文件菜单槽
    void onActionLoadConfig();    // 加载配置文件
    void onActionSaveConfig();    // 保存配置文件
    void onActionEditConfig();    // 打开配置编辑对话框
    void onActionSelectSaveDir(); // 选择CSV保存目录

    //  视图菜单槽
    void onActionSetYRange();  // 打开Y轴范围设置对话框
    void onActionBoxZoom();     // 开启一次性框选放大模式
    void onActionResetView();   // 重置视图（X自动滚动+Y自动缩放）

    //  通讯菜单槽
    void onActionConnect();    // 打开连接对话框并连接
    void onActionDisconnect(); // 断开Modbus连接

    //  通讯模块信号槽
    void onConnectionChanged(bool connected, const QString& msg); // 更新状态栏连接状态
    void onCommError(const QString& err);                         // 显示通讯错误提示

private:
    Ui::MainWindow *ui;

    //  业务模块
    ConfigManager   _cfg;       // 配置管理器
    ModbusComm      _comm;      // Modbus通讯模块
    DataParser*     _parser = nullptr; // 数据解析器
    CsvLogger       _logger;    // CSV数据记录器

    //  UI控件
    CurveView*      _curveView = nullptr; // 实时曲线视图
    ControlPanel*   _control = nullptr;    // 控制面板（点位选择+下发）
    CurveSelector*  _selector = nullptr;   // 曲线选择器（勾选可见点位）
    QSplitter*      _topSplit = nullptr;   // 上方左右分栏
    QSplitter*      _mainSplit = nullptr;  // 上下整体分栏
    QLabel*         _lblConn = nullptr;    // 状态栏连接状态标签

    //  偏好设置（保存到QSettings）
    QString _lastConfigPath;          // 上次加载的配置文件路径
    QString _lastSaveDir;             // 上次选择的CSV保存目录
    QString _lastIp = "127.0.0.1";   // 上次连接的IP地址
    quint16 _lastPort = 502;          // 上次连接的端口号
    int     _lastSlaveId = 1;         // 上次连接的从站号

    //  内部辅助函数
    void loadSettings();       // 从QSettings加载偏好设置
    void saveSettings();       // 保存偏好设置到QSettings
    void tryAutoLoadConfig();  // 尝试自动加载上次的配置文件
    bool doLoadConfig(const QString& path); // 执行加载配置文件
    bool doSaveConfig(const QString& path); // 执行保存配置文件
    void setupLayout();        // 搭建主窗口布局
    void onConfigChanged();    // 配置变化后刷新各UI模块
};

#endif // MAINWINDOW_H