#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>

// 前向声明
namespace Ui { class ControlPanel; }
class ConfigManager;

// 控制面板：用于选择可控点位并下发寄存器值
class ControlPanel : public QWidget {
    Q_OBJECT
public:
    explicit ControlPanel(QWidget* parent = nullptr);
    ~ControlPanel();

    // 设置配置管理器
    void setConfigManager(ConfigManager* cfg) { _cfg = cfg; }

public slots:
    // 配置变化时调用：刷新下拉框选项
    void refresh();
    // 写入完成回调：更新状态文字
    void onWriteFinished(quint16 reg, bool ok, const QString& err);

signals:
    // 用户点击下发按钮时发送（reg是寄存器地址，value是raw值）
    void writeRequested(quint16 reg, quint16 value);

private slots:
    void onComboChanged(int);
    void onSendClicked();

private:
    Ui::ControlPanel* ui;
    ConfigManager* _cfg = nullptr;

    // 根据当前选中点位的倍率调整输入框范围和小数位数
    void applyPointSpinRange();
};

#endif // CONTROLPANEL_H