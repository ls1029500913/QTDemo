#ifndef CURVESELECTOR_H
#define CURVESELECTOR_H

#include <QWidget>
#include <QStringList>

// 前向声明
class QListWidget;
class QListWidgetItem;
class ConfigManager;

// 曲线选择面板：列出所有点位，支持勾选/全选/全不选
class CurveSelector : public QWidget {
    Q_OBJECT
public:
    explicit CurveSelector(QWidget* parent = nullptr);

    // 设置配置管理器
    void setConfigManager(ConfigManager* cfg) { _cfg = cfg; }

public slots:
    // 配置变化时调用：重建列表，默认全部勾选
    void refresh();

signals:
    // 勾选状态变化时发送（当前所有勾选的点位名列表）
    void visibleTagsChanged(QStringList tags);

private slots:
    void onItemChanged(QListWidgetItem* item);
    void onSelectAll();
    void onSelectNone();

private:
    ConfigManager* _cfg = nullptr;
    QListWidget* _list;

    // 防止刷新时触发 itemChanged 信号导致重复 emit
    bool _populating = false;

    // 内部辅助函数
    QStringList currentChecked() const; // 获取当前勾选的点位列表
    void emitChange(); // 发送 visibleTagsChanged 信号
};

#endif // CURVESELECTOR_H