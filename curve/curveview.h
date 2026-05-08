#ifndef CURVEVIEW_H
#define CURVEVIEW_H

#include <QWidget>
#include <QHash>
#include <QSet>
#include <QStringList>
#include <QList>
#include "data/point_value.h"

// 前向声明
class QCustomPlot; // 曲线绘图库
class QCPGraph;    // 单条曲线
class QTimer;      // 定时器

// 实时曲线视图
class CurveView : public QWidget {
    Q_OBJECT
public:
    explicit CurveView(QWidget* parent = nullptr);

    //  公共接口
    // 配置变化时调用，清空所有曲线
    void resetGraphs();

    // Y 轴缩放控制
    double yMin() const; // 获取 Y 轴最小值
    double yMax() const; // 获取 Y 轴最大值
    bool yAutoScale() const { return _yAutoScale; } // Y 是否自动缩放

    void setYAutoScale(bool on); // 开启/关闭 Y 轴自动缩放
    void setYRange(double mn, double mx); // 手动设置 Y 轴范围

    // 开启一次性框选放大模式
    void beginBoxZoom();

    // 重置视图（X 自动滚动 + Y 自动缩放）
    void resetView();

public slots:
    // 接收解析后的实时点位数据，更新曲线
    void onPointsReceived(QList<PointValue> points);
    // 设置可见的点位标签（只显示列表中的曲线）
    void setVisibleTags(const QStringList& tags);

private slots:
    // 轮询检查框选放大流程
    // 因为 QCustomPlot 的 mouseRelease 信号在 srmZoom 模式下不发出，
    // 也用不了事件过滤器（被 axis rect 内部吞掉），
    // 所以用定时器 100ms 检查一次"按下→松开"序列
    void checkBoxZoomDone();

private:
    //  核心成员
    QCustomPlot* _plot = nullptr;
    QHash<QString, QCPGraph*> _graphs; // 点位名 -> 曲线对象

    //  可见性控制
    QSet<QString> _visibleTags;
    bool _hasVisibleFilter = false;

    //  曲线参数
    int _maxPoints = 50000;    // 单条曲线最大点数
    double _windowSec = 60.0;   // X 轴显示窗口（秒）

    //  视图状态
    bool _yAutoScale = true;    // Y 是否自动缩放
    bool _xUserZoomed = false;  // 用户是否手动调整过 X 轴（停止自动滚动）

    //  框选放大流程
    QTimer* _zoomCheckTimer = nullptr;
    bool _zoomSeenPress = false; // 已经看到鼠标按下了吗？false=等按下，true=等松开

    //  内部辅助函数
    QCPGraph* ensureGraph(const QString& tag); // 获取或创建指定名称的曲线
    bool isTagVisible(const QString& tag) const; // 判断该点位是否需要显示
    void setupPlot(); // 初始化绘图控件样式、交互
};

#endif // CURVEVIEW_H