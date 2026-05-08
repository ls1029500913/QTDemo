#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

// 前向声明
namespace Ui { class ConnectDialog; }

// 连接对话框：用于输入 Modbus TCP 的 IP、端口、从站号
class ConnectDialog : public QDialog {
    Q_OBJECT
public:
    explicit ConnectDialog(QWidget* parent = nullptr);
    ~ConnectDialog();

    //  取值接口
    // 获取用户输入的 IP 地址（自动去除首尾空格）
    QString ip() const;
    // 获取用户输入的端口号
    quint16 port() const;
    // 获取用户输入的从站号
    int slaveId() const;

    //  预填接口
    // 弹窗前预填上次的 IP 地址
    void setIp(const QString& ip);
    // 弹窗前预填上次的端口号
    void setPort(quint16 port);
    // 弹窗前预填上次的从站号
    void setSlaveId(int id);

private:
    Ui::ConnectDialog* ui;
};

#endif // CONNECTDIALOG_H