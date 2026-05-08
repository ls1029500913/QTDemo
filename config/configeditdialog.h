#ifndef CONFIGEDITDIALOG_H
#define CONFIGEDITDIALOG_H

#include <QDialog>
#include "config/point_config.h"

// 前向声明
namespace Ui { class ConfigEditDialog; }

// 配置编辑对话框
class ConfigEditDialog : public QDialog {
    Q_OBJECT
public:
    explicit ConfigEditDialog(QWidget* parent = nullptr);
    ~ConfigEditDialog();

    // 加载现有配置到表格
    void setData(const QList<PointConfig>& list);
    // 获取用户编辑结果（仅在 accept 后有效）
    QList<PointConfig> getData() const { return _result; }

private slots:
    void onAddRow();
    void onDelRow();
    void onAccept();

private:
    Ui::ConfigEditDialog* ui;
    QList<PointConfig> _result; // 校验通过的最终结果

    // 内部辅助函数
    void initTable();
    void appendRow(const PointConfig& p);
    QList<PointConfig> readTable() const;
};

#endif // CONFIGEDITDIALOG_H