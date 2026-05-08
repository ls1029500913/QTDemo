#include "curve/curveview.h"
#include "qcustomplot.h"

#include <QVBoxLayout>
#include <QDateTime>
#include <QTimer>
#include <QApplication>

// 构造函数：初始化绘图控件
CurveView::CurveView(QWidget* parent) : QWidget(parent)
{
    setupPlot();
}

// 初始化 QCustomPlot：坐标轴、交互、样式、信号绑定
void CurveView::setupPlot()
{
    _plot = new QCustomPlot(this);

    // 布局：充满窗口，无边距
    auto* lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->addWidget(_plot);

    // X 轴：时间格式
    auto timeTicker = QSharedPointer<QCPAxisTickerDateTime>::create();
    timeTicker->setDateTimeFormat("HH:mm:ss");
    _plot->xAxis->setTicker(timeTicker);
    _plot->xAxis->setLabel(QString::fromUtf8("时间"));
    _plot->yAxis->setLabel(QString::fromUtf8("值"));

    // 开启鼠标交互：拖动平移 + 滚轮缩放
    _plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    _plot->setSelectionRectMode(QCP::srmNone); // 默认不开启框选

    // 显示图例
    _plot->legend->setVisible(true);
    _plot->legend->setFont(QFont("Microsoft YaHei", 9));

    // 检测用户手动操作：停止X轴自动滚动
    connect(_plot, &QCustomPlot::mousePress, this, [this](QMouseEvent*){
        if (_plot->selectionRectMode() != QCP::srmZoom)
            _xUserZoomed = true;
    });
    connect(_plot, &QCustomPlot::mouseWheel, this, [this](QWheelEvent*){
        _xUserZoomed = true;
    });

    // 双击：重置视图
    connect(_plot, &QCustomPlot::mouseDoubleClick, this, [this](QMouseEvent*){
        resetView();
    });

    // 框选放大定时器：解决QCustomPlot不触发mouseRelease的问题
    _zoomCheckTimer = new QTimer(this);
    _zoomCheckTimer->setInterval(100);
    connect(_zoomCheckTimer, &QTimer::timeout, this, &CurveView::checkBoxZoomDone);
}

// 重置所有曲线：清空图表、清除缓存、恢复自动状态
void CurveView::resetGraphs()
{
    _plot->clearGraphs();
    _graphs.clear();
    _xUserZoomed = false;
    _yAutoScale = true;
    _plot->replot();
}

// 获取当前Y轴范围
double CurveView::yMin() const { return _plot->yAxis->range().lower; }
double CurveView::yMax() const { return _plot->yAxis->range().upper; }

// 设置Y轴自动缩放
void CurveView::setYAutoScale(bool on)
{
    _yAutoScale = on;
    _plot->replot(QCustomPlot::rpQueuedReplot);
}

// 手动设置Y轴范围（关闭自动缩放）
void CurveView::setYRange(double mn, double mx)
{
    _yAutoScale = false;
    _plot->yAxis->setRange(mn, mx);
    _plot->replot(QCustomPlot::rpQueuedReplot);
}

// 开启一次框选放大模式
void CurveView::beginBoxZoom()
{
    qDebug() << "[zoom] beginBoxZoom called, before mode:" << _plot->selectionRectMode();
    _plot->setSelectionRectMode(QCP::srmZoom);
    qDebug() << "[zoom] after  mode:" << _plot->selectionRectMode();

    _zoomSeenPress = false; // 复位按下标志
    _zoomCheckTimer->start(); // 启动监听定时器
}

// 重置视图：恢复X自动滚动、Y自动缩放、显示最近60秒
void CurveView::resetView()
{
    _xUserZoomed = false;
    _yAutoScale = true;

    // X轴显示最近 windowSec 秒数据
    double now = QDateTime::currentMSecsSinceEpoch() / 1000.0;
    _plot->xAxis->setRange(now - _windowSec, now);

    // Y轴自动适配所有曲线范围
    if (!_graphs.isEmpty())
        _plot->yAxis->rescale();

    _plot->replot(QCustomPlot::rpQueuedReplot);
}

// 轮询检查框选流程
void CurveView::checkBoxZoomDone()
{
    // 不在框选模式 → 停定时器
    if (_plot->selectionRectMode() != QCP::srmZoom) {
        _zoomCheckTimer->stop();
        _zoomSeenPress = false;
        return;
    }

    // 获取当前鼠标左键是否按下
    const bool down = QApplication::mouseButtons() & Qt::LeftButton;

    // 第一阶段：等待用户按下左键
    if (!_zoomSeenPress) {
        if (down)
            _zoomSeenPress = true;
        return;
    }

    // 第二阶段：等待用户松开左键
    if (down) return;

    // 左键松开 → 框选完成，切回平移模式
    qDebug() << "[zoom] box zoom finished, switch back to pan mode";
    _plot->setSelectionRectMode(QCP::srmNone);
    _xUserZoomed = true;
    _yAutoScale = false;

    _zoomCheckTimer->stop();
    _zoomSeenPress = false;
}

// 判断某个点位是否需要显示
bool CurveView::isTagVisible(const QString& tag) const
{
    if (!_hasVisibleFilter) return true;
    return _visibleTags.contains(tag);
}

// 设置可见点位列表（同步曲线和图例）
void CurveView::setVisibleTags(const QStringList& tags)
{
    _hasVisibleFilter = true;
    _visibleTags = QSet<QString>(tags.begin(), tags.end());

    for (auto it = _graphs.begin(); it != _graphs.end(); ++it) {
        const bool on = _visibleTags.contains(it.key());
        it.value()->setVisible(on);
        if (auto* item = _plot->legend->itemWithPlottable(it.value()))
            item->setVisible(on);
    }
    _plot->replot(QCustomPlot::rpQueuedReplot);
}

// 接收实时数据，添加到曲线并刷新界面
void CurveView::onPointsReceived(QList<PointValue> points)
{
    for (const PointValue& pv : points) {
        QCPGraph* g = ensureGraph(pv.name);

        // 时间戳转秒
        double tSec = pv.timestampMs / 1000.0;
        g->addData(tSec, pv.value);

        // 数据点超限：批量删除最老的1000点
        if (g->data()->size() > _maxPoints + 1000) {
            auto it = g->data()->constBegin();
            for (int i = 0; i < 1000 && it != g->data()->constEnd(); ++i)
                ++it;
            if (it != g->data()->constEnd())
                g->data()->removeBefore(it->key);
        }
    }

    // X 轴：没手动调过才自动滚动
    if (!_xUserZoomed) {
        double now = QDateTime::currentMSecsSinceEpoch() / 1000.0;
        _plot->xAxis->setRange(now - _windowSec, now);
    }

    // Y 轴：自动模式才 rescale
    if (_yAutoScale)
        _plot->yAxis->rescale();

    _plot->replot(QCustomPlot::rpQueuedReplot);
}

// 确保曲线存在：不存在则创建，存在则直接返回
QCPGraph* CurveView::ensureGraph(const QString& tag)
{
    // 已存在 → 直接返回
    auto it = _graphs.find(tag);
    if (it != _graphs.end())
        return it.value();

    // 不存在 → 创建新曲线
    auto* g = _plot->addGraph();
    g->setName(tag);
    g->setAdaptiveSampling(true); // 自适应采样，大量数据不卡顿

    // 配色方案：循环使用固定颜色列表
    static const QList<QColor> palette = {
        QColor(220,  60,  60), QColor( 60, 130, 220), QColor( 60, 180,  80),
        QColor(220, 150,  50), QColor(160,  80, 200), QColor( 50, 180, 200),
        QColor(220, 100, 160), QColor(120, 120,  60),
    };
    QPen pen(palette.at(_graphs.size() % palette.size()));
    pen.setWidth(2);
    g->setPen(pen);

    // 根据过滤规则设置显隐
    const bool visible = isTagVisible(tag);
    g->setVisible(visible);
    if (auto* item = _plot->legend->itemWithPlottable(g))
        item->setVisible(visible);

    // 存入哈希表
    _graphs.insert(tag, g);
    return g;
}