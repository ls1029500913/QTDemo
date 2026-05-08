#include "connectdialog.h"
#include "ui_connectdialog.h"

// 构造函数：初始化 UI
ConnectDialog::ConnectDialog(QWidget* parent)
    : QDialog(parent), ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
}

// 析构函数：释放 UI
ConnectDialog::~ConnectDialog()
{
    delete ui;
}

// 获取用户输入的 IP 地址
QString ConnectDialog::ip() const
{
    return ui->lineIp->text().trimmed();
}

// 获取用户输入的端口号
quint16 ConnectDialog::port() const
{
    return static_cast<quint16>(ui->spinPort->value());
}

// 获取用户输入的从站号
int ConnectDialog::slaveId() const
{
    return ui->spinSlaveId->value();
}

// 设置 IP 地址到输入框
void ConnectDialog::setIp(const QString& ip)
{
    ui->lineIp->setText(ip);
}

// 设置端口号到输入框
void ConnectDialog::setPort(quint16 port)
{
    ui->spinPort->setValue(port);
}

// 设置从站号到输入框
void ConnectDialog::setSlaveId(int id)
{
    ui->spinSlaveId->setValue(id);
}