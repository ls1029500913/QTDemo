#include "controlpanel.h"
#include "ui_controlpanel.h"
#include "config/configmanager.h"
#include "config/point_config.h"

#include <QPushButton>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <cmath>
#include <limits>

// 构造函数：初始化 UI、连接信号槽、设置默认输入范围
ControlPanel::ControlPanel(QWidget* parent)
    : QWidget(parent), ui(new Ui::ControlPanel)
{
    ui->setupUi(this);

    // 连接信号槽
    connect(ui->comboPoint, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ControlPanel::onComboChanged);
    connect(ui->btnSend, &QPushButton::clicked,
            this, &ControlPanel::onSendClicked);

    // 设置输入框默认精度和范围
    ui->spinValue->setDecimals(4);
    ui->spinValue->setRange(-1e9, 1e9);
}

// 析构函数：释放 UI
ControlPanel::~ControlPanel()
{
    delete ui;
}

// 刷新控制面板：重新加载可控制点位列表
void ControlPanel::refresh()
{
    // 暂停信号，避免 clear 时触发 onComboChanged
    ui->comboPoint->blockSignals(true);
    ui->comboPoint->clear();

    if (_cfg) {
        // 只添加标记为"可控制"的点位
        for (const PointConfig& p : _cfg->points()) {
            if (!p.controllable)
                continue;
            // userData 存储寄存器地址，下发时直接取用
            ui->comboPoint->addItem(p.name, p.reg);
        }
    }

    ui->comboPoint->blockSignals(false);

    // 根据是否有点位设置控件状态
    const bool any = ui->comboPoint->count() > 0;
    ui->spinValue->setEnabled(any);
    ui->btnSend->setEnabled(any);

    if (any) {
        ui->labelStatus->setText(QString::fromUtf8("就绪"));
        applyPointSpinRange(); // 按当前点位调整输入范围
    } else {
        ui->labelStatus->setText(QString::fromUtf8("无可下发点位"));
    }
}

// 下拉框变化：调整输入框范围
void ControlPanel::onComboChanged(int)
{
    applyPointSpinRange();
}

// 根据当前选中点位调整输入框的范围和小数位数
void ControlPanel::applyPointSpinRange()
{
    if (!_cfg) return;
    if (ui->comboPoint->count() == 0) return;

    // 找到当前选中的点位配置
    const QString name = ui->comboPoint->currentText();
    const PointConfig* p = nullptr;
    for (const PointConfig& pc : _cfg->points()) {
        if (pc.name == name) {
            p = &pc;
            break;
        }
    }
    if (!p) return;

    const double s = std::abs(p->scale);

    // 计算工程值范围：INT16 范围是 -32768~32767，乘以倍率
    const double mn = -32768.0 * s;
    const double mx = 32767.0 * s;
    ui->spinValue->setRange(mn, mx);
    ui->spinValue->setValue(0);

    // 推算小数位数：倍率 0.01 → 2 位小数；倍率 1 → 0 位
    int dec = 0;
    double t = s;
    while (dec < 6 && t > 1e-9 && std::abs(t - std::round(t)) > 1e-9) {
        t *= 10.0;
        dec++;
    }
    ui->spinValue->setDecimals(dec);
}

// 点击下发按钮：工程值转 raw 值并发送
void ControlPanel::onSendClicked()
{
    if (!_cfg) return;
    if (ui->comboPoint->count() == 0) return;

    // 找到当前选中的点位配置
    const QString name = ui->comboPoint->currentText();
    const PointConfig* p = nullptr;
    for (const PointConfig& pc : _cfg->points()) {
        if (pc.name == name) {
            p = &pc;
            break;
        }
    }
    if (!p) return;

    const double eng = ui->spinValue->value();
    if (p->scale == 0.0) {
        ui->labelStatus->setText(QString::fromUtf8("倍率为 0，无法下发"));
        return;
    }

    // 工程值转 raw 值：四舍五入
    double rawd = std::round(eng / p->scale);

    // 限制在 INT16 范围内
    if (rawd < std::numeric_limits<qint16>::min())
        rawd = std::numeric_limits<qint16>::min();
    if (rawd > std::numeric_limits<qint16>::max())
        rawd = std::numeric_limits<qint16>::max();

    // 类型转换
    const qint16 raw_signed = static_cast<qint16>(rawd);
    const quint16 raw = static_cast<quint16>(raw_signed);

    // 更新 UI 状态并发送信号
    ui->labelStatus->setText(QString::fromUtf8("下发中..."));
    ui->btnSend->setEnabled(false);
    emit writeRequested(p->reg, raw);
}

// 写入完成回调：更新状态
void ControlPanel::onWriteFinished(quint16 reg, bool ok, const QString& err)
{
    Q_UNUSED(reg);
    ui->btnSend->setEnabled(true);
    if (ok) {
        ui->labelStatus->setText(QString::fromUtf8("下发成功"));
    } else {
        ui->labelStatus->setText(QString::fromUtf8("下发失败：") + err);
    }
}