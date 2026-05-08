#include "curveselector.h"
#include "config/configmanager.h"
#include "config/point_config.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

// 构造函数：创建 UI 布局、控件、连接信号槽
CurveSelector::CurveSelector(QWidget* parent) : QWidget(parent)
{
    auto* vlay = new QVBoxLayout(this);

    // 标题
    auto* title = new QLabel(QString::fromUtf8("曲线选择"), this);
    title->setStyleSheet("font-weight:bold; font-size:13px; padding:2px;");
    vlay->addWidget(title);

    // 全选/全不选按钮行
    auto* btnRow = new QHBoxLayout();
    auto* btnAll = new QPushButton(QString::fromUtf8("全选"), this);
    auto* btnNone = new QPushButton(QString::fromUtf8("全不选"), this);
    btnRow->addWidget(btnAll);
    btnRow->addWidget(btnNone);
    btnRow->addStretch();
    vlay->addLayout(btnRow);

    // 点位列表
    _list = new QListWidget(this);
    vlay->addWidget(_list, 1); // stretch=1 让列表撑满剩余空间

    // 连接信号槽
    connect(_list, &QListWidget::itemChanged, this, &CurveSelector::onItemChanged);
    connect(btnAll, &QPushButton::clicked, this, &CurveSelector::onSelectAll);
    connect(btnNone, &QPushButton::clicked, this, &CurveSelector::onSelectNone);
}

// 刷新列表：重建所有点位项，默认全部勾选
void CurveSelector::refresh()
{
    _populating = true; // 标记正在刷新，忽略 itemChanged 信号
    _list->clear();

    if (_cfg) {
        for (const PointConfig& p : _cfg->points()) {
            auto* item = new QListWidgetItem(p.name, _list);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setCheckState(Qt::Checked); // 默认全选
        }
    }

    _populating = false;
    emitChange(); // 通知 CurveView 更新可见曲线
}

// 获取当前选中的点位名称列表
QStringList CurveSelector::currentChecked() const
{
    QStringList out;
    for (int i = 0; i < _list->count(); ++i) {
        auto* it = _list->item(i);
        if (it->checkState() == Qt::Checked)
            out << it->text();
    }
    return out;
}

// 发送可见标签变化信号
void CurveSelector::emitChange()
{
    emit visibleTagsChanged(currentChecked());
}

// 列表项变化：刷新期间忽略，否则发送信号
void CurveSelector::onItemChanged(QListWidgetItem*)
{
    if (_populating) return;
    emitChange();
}

// 全选：设置所有项为勾选状态
void CurveSelector::onSelectAll()
{
    _populating = true;
    for (int i = 0; i < _list->count(); ++i)
        _list->item(i)->setCheckState(Qt::Checked);
    _populating = false;
    emitChange();
}

// 全不选：设置所有项为未勾选状态
void CurveSelector::onSelectNone()
{
    _populating = true;
    for (int i = 0; i < _list->count(); ++i)
        _list->item(i)->setCheckState(Qt::Unchecked);
    _populating = false;
    emitChange();
}