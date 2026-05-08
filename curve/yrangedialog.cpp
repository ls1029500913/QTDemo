#include "yrangedialog.h"
#include "ui_yrangedialog.h"

#include <QMessageBox>

// 构造函数：初始化 UI、设置输入范围、绑定信号槽
YRangeDialog::YRangeDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::YRangeDialog)
{
    ui->setupUi(this);

    // 设置输入范围：极大值，防止用户输入大数被限制
    ui->spinMin->setRange(-1e9, 1e9);
    ui->spinMax->setRange(-1e9, 1e9);
    // 小数点精度：4 位
    ui->spinMin->setDecimals(4);
    ui->spinMax->setDecimals(4);

    // 勾选【自动缩放】时，禁用手动输入框
    connect(ui->checkAuto, &QCheckBox::toggled, this, [this](bool on){
        ui->spinMin->setEnabled(!on);
        ui->spinMax->setEnabled(!on);
    });

    // 绑定按钮：确定 → 校验并关闭；取消 → 直接关闭
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &YRangeDialog::onAccept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

// 析构函数：释放 UI
YRangeDialog::~YRangeDialog()
{
    delete ui;
}

// 外部设置弹窗初始值：最小值、最大值
void YRangeDialog::setRange(double mn, double mx)
{
    ui->spinMin->setValue(mn);
    ui->spinMax->setValue(mx);
}

// 外部设置【自动缩放】勾选状态
void YRangeDialog::setAutoScale(bool on)
{
    ui->checkAuto->setChecked(on);
    ui->spinMin->setEnabled(!on);
    ui->spinMax->setEnabled(!on);
}

// 获取用户输入的最小值
double YRangeDialog::minValue() const
{
    return ui->spinMin->value();
}

// 获取用户输入的最大值
double YRangeDialog::maxValue() const
{
    return ui->spinMax->value();
}

// 是否开启自动缩放
bool YRangeDialog::autoScale() const
{
    return ui->checkAuto->isChecked();
}

// 点击【确定】时的校验逻辑
void YRangeDialog::onAccept()
{
    // 手动模式下必须满足：最小值 < 最大值
    if (!autoScale() && minValue() >= maxValue()) {
        QMessageBox::warning(this, QString::fromUtf8("范围错误"),
                             QString::fromUtf8("最小值必须小于最大值"));
        return; // 校验不通过，不关闭窗口
    }
    accept(); // 校验通过，关闭弹窗
}