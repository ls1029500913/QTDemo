#include "config/configeditdialog.h"
#include "ui_configeditdialog.h"
#include "config/configmanager.h"

#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMessageBox>
#include <algorithm>

// 构造函数：初始化 UI、表格、绑定信号槽
ConfigEditDialog::ConfigEditDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::ConfigEditDialog)
{
    ui->setupUi(this);
    initTable();

    // 绑定按钮信号
    connect(ui->btnAdd, &QPushButton::clicked, this, &ConfigEditDialog::onAddRow);
    connect(ui->btnDel, &QPushButton::clicked, this, &ConfigEditDialog::onDelRow);
    connect(ui->btnOk, &QPushButton::clicked, this, &ConfigEditDialog::onAccept);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

// 析构函数：释放 UI
ConfigEditDialog::~ConfigEditDialog()
{
    delete ui;
}

// 初始化配置表格：列数、表头、样式
void ConfigEditDialog::initTable()
{
    ui->table->setColumnCount(4);
    QStringList headers;
    headers << QString::fromUtf8("点位名称")
            << QString::fromUtf8("寄存器")
            << QString::fromUtf8("倍率")
            << QString::fromUtf8("是否可控制");
    ui->table->setHorizontalHeaderLabels(headers);

    // 表头自适应拉伸
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 选中整行
    ui->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 交替行颜色
    ui->table->setAlternatingRowColors(true);
}

// 加载现有配置到表格
void ConfigEditDialog::setData(const QList<PointConfig>& list)
{
    ui->table->setRowCount(0); // 先清空
    for (const auto& p : list)
        appendRow(p);
}

// 向表格追加一行配置
void ConfigEditDialog::appendRow(const PointConfig& p)
{
    int r = ui->table->rowCount();
    ui->table->insertRow(r);

    // 第1列：点位名称
    ui->table->setItem(r, 0, new QTableWidgetItem(p.name));
    // 第2列：寄存器地址
    ui->table->setItem(r, 1, new QTableWidgetItem(QString::number(p.reg)));
    // 第3列：倍率
    ui->table->setItem(r, 2, new QTableWidgetItem(QString::number(p.scale, 'g', 10)));

    // 第4列：是否可控制（复选框，不可编辑）
    auto* itemCtrl = new QTableWidgetItem();
    itemCtrl->setFlags(itemCtrl->flags() | Qt::ItemIsUserCheckable);
    itemCtrl->setFlags(itemCtrl->flags() & ~Qt::ItemIsEditable);
    itemCtrl->setCheckState(p.controllable ? Qt::Checked : Qt::Unchecked);
    itemCtrl->setTextAlignment(Qt::AlignCenter);
    ui->table->setItem(r, 3, itemCtrl);
}

// 添加新行：默认名称、倍率1.0
void ConfigEditDialog::onAddRow()
{
    PointConfig p;
    p.name = QString::fromUtf8("点位%1").arg(ui->table->rowCount() + 1);
    p.scale = 1.0;
    appendRow(p);
}

// 删除选中行（未选中则删最后一行）
void ConfigEditDialog::onDelRow()
{
    auto rows = ui->table->selectionModel()->selectedRows();
    if (rows.isEmpty()) {
        // 未选中行，删除最后一行
        if (ui->table->rowCount() > 0)
            ui->table->removeRow(ui->table->rowCount() - 1);
        return;
    }

    // 收集选中行索引
    QList<int> idxs;
    for (const auto& m : rows)
        idxs.append(m.row());

    // 从后往前删，避免索引错乱
    std::sort(idxs.begin(), idxs.end(), std::greater<int>());
    for (int r : idxs)
        ui->table->removeRow(r);
}

// 从表格读取所有配置
QList<PointConfig> ConfigEditDialog::readTable() const
{
    QList<PointConfig> list;
    for (int r = 0; r < ui->table->rowCount(); ++r) {
        PointConfig p;
        p.name = ui->table->item(r, 0)->text().trimmed();
        p.reg = static_cast<quint16>(ui->table->item(r, 1)->text().trimmed().toInt());
        p.scale = ui->table->item(r, 2)->text().trimmed().toDouble();
        p.controllable = (ui->table->item(r, 3)->checkState() == Qt::Checked);
        list.append(p);
    }
    return list;
}

// 确定按钮：校验配置后关闭
void ConfigEditDialog::onAccept()
{
    auto list = readTable();

    // 临时 ConfigManager 用于校验
    ConfigManager temp;
    temp.setPoints(list);
    QString err = temp.validate();
    if (!err.isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("校验失败"), err);
        return; // 校验不通过，不关闭窗口
    }

    // 校验通过，保存结果并关闭
    _result = list;
    accept();
}