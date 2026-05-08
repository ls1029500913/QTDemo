#ifndef YRANGEDIALOG_H
#define YRANGEDIALOG_H

#include <QDialog>

// 前向声明
namespace Ui { class YRangeDialog; }

// 设置 Y 轴范围的小弹窗
// UI：两个数值输入框（最小值/最大值）+ 一个"自动缩放"复选框 + 确定/取消按钮
class YRangeDialog : public QDialog {
    Q_OBJECT
public:
    explicit YRangeDialog(QWidget* parent = nullptr);
    ~YRangeDialog();

    //  弹窗预填值
    // 弹窗前预填当前 Y 轴范围
    void setRange(double mn, double mx);
    // 弹窗前预填当前是否自动缩放
    void setAutoScale(bool on);

    //  用户取值
    // 用户点确定后获取输入的最小值
    double minValue() const;
    // 用户点确定后获取输入的最大值
    double maxValue() const;
    // 用户点确定后获取是否勾选了自动缩放
    bool autoScale() const;

private slots:
    // 点击确定按钮时的校验逻辑（手动模式下必须满足 min < max）
    void onAccept();

private:
    Ui::YRangeDialog* ui;
};

#endif // YRANGEDIALOG_H