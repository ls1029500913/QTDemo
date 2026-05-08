#include "mainwindow/mainwindow.h"
#include "config/configmanager.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("YourName");
    QCoreApplication::setApplicationName("ModbusTester");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}