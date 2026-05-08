/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionEditConfig;
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionTestWrite;
    QAction *actionLoadConfig;
    QAction *actionSaveConfig;
    QAction *actionSelectSaveDir;
    QAction *actionSetYRange;
    QAction *actionBoxZoom;
    QAction *actionResetView;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        actionEditConfig = new QAction(MainWindow);
        actionEditConfig->setObjectName(QString::fromUtf8("actionEditConfig"));
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        actionDisconnect = new QAction(MainWindow);
        actionDisconnect->setObjectName(QString::fromUtf8("actionDisconnect"));
        actionTestWrite = new QAction(MainWindow);
        actionTestWrite->setObjectName(QString::fromUtf8("actionTestWrite"));
        actionLoadConfig = new QAction(MainWindow);
        actionLoadConfig->setObjectName(QString::fromUtf8("actionLoadConfig"));
        actionSaveConfig = new QAction(MainWindow);
        actionSaveConfig->setObjectName(QString::fromUtf8("actionSaveConfig"));
        actionSelectSaveDir = new QAction(MainWindow);
        actionSelectSaveDir->setObjectName(QString::fromUtf8("actionSelectSaveDir"));
        actionSetYRange = new QAction(MainWindow);
        actionSetYRange->setObjectName(QString::fromUtf8("actionSetYRange"));
        actionBoxZoom = new QAction(MainWindow);
        actionBoxZoom->setObjectName(QString::fromUtf8("actionBoxZoom"));
        actionResetView = new QAction(MainWindow);
        actionResetView->setObjectName(QString::fromUtf8("actionResetView"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menu->addAction(actionEditConfig);
        menu->addAction(actionLoadConfig);
        menu->addAction(actionSaveConfig);
        menu->addAction(actionSelectSaveDir);
        menu_2->addAction(actionConnect);
        menu_2->addAction(actionDisconnect);
        menu_3->addAction(actionSetYRange);
        menu_3->addAction(actionBoxZoom);
        menu_3->addAction(actionResetView);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionEditConfig->setText(QCoreApplication::translate("MainWindow", "\347\274\226\350\276\221\351\205\215\347\275\256", nullptr));
#if QT_CONFIG(shortcut)
        actionEditConfig->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+E", nullptr));
#endif // QT_CONFIG(shortcut)
        actionConnect->setText(QCoreApplication::translate("MainWindow", "\350\277\236\346\216\245 PLC", nullptr));
        actionDisconnect->setText(QCoreApplication::translate("MainWindow", "\346\226\255\345\274\200", nullptr));
        actionTestWrite->setText(QCoreApplication::translate("MainWindow", "\345\206\231 reg 0 = 9999\n"
"", nullptr));
        actionLoadConfig->setText(QCoreApplication::translate("MainWindow", "\345\212\240\350\275\275\351\205\215\347\275\256", nullptr));
#if QT_CONFIG(shortcut)
        actionLoadConfig->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSaveConfig->setText(QCoreApplication::translate("MainWindow", "\344\277\235\345\255\230\351\205\215\347\275\256", nullptr));
#if QT_CONFIG(shortcut)
        actionSaveConfig->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSelectSaveDir->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\344\277\235\345\255\230\347\233\256\345\275\225", nullptr));
#if QT_CONFIG(shortcut)
        actionSelectSaveDir->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+D", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSetYRange->setText(QCoreApplication::translate("MainWindow", "\350\256\276\347\275\256 Y \350\275\264\350\214\203\345\233\264", nullptr));
#if QT_CONFIG(shortcut)
        actionSetYRange->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Y", nullptr));
#endif // QT_CONFIG(shortcut)
        actionBoxZoom->setText(QCoreApplication::translate("MainWindow", "\346\241\206\351\200\211\346\224\276\345\244\247", nullptr));
#if QT_CONFIG(shortcut)
        actionBoxZoom->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+B", nullptr));
#endif // QT_CONFIG(shortcut)
        actionResetView->setText(QCoreApplication::translate("MainWindow", "\351\207\215\347\275\256\350\247\206\345\233\276", nullptr));
#if QT_CONFIG(shortcut)
        actionResetView->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+R", nullptr));
#endif // QT_CONFIG(shortcut)
        menu->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        menu_2->setTitle(QCoreApplication::translate("MainWindow", "\351\200\232\350\256\257", nullptr));
        menu_3->setTitle(QCoreApplication::translate("MainWindow", "\350\247\206\345\233\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
