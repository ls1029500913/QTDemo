/********************************************************************************
** Form generated from reading UI file 'configeditdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGEDITDIALOG_H
#define UI_CONFIGEDITDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConfigEditDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *hint;
    QTableWidget *table;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnAdd;
    QPushButton *btnDel;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnOk;
    QPushButton *btnCancel;

    void setupUi(QDialog *ConfigEditDialog)
    {
        if (ConfigEditDialog->objectName().isEmpty())
            ConfigEditDialog->setObjectName(QString::fromUtf8("ConfigEditDialog"));
        ConfigEditDialog->resize(720, 460);
        verticalLayout = new QVBoxLayout(ConfigEditDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        hint = new QLabel(ConfigEditDialog);
        hint->setObjectName(QString::fromUtf8("hint"));

        verticalLayout->addWidget(hint);

        table = new QTableWidget(ConfigEditDialog);
        table->setObjectName(QString::fromUtf8("table"));

        verticalLayout->addWidget(table);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btnAdd = new QPushButton(ConfigEditDialog);
        btnAdd->setObjectName(QString::fromUtf8("btnAdd"));

        horizontalLayout->addWidget(btnAdd);

        btnDel = new QPushButton(ConfigEditDialog);
        btnDel->setObjectName(QString::fromUtf8("btnDel"));

        horizontalLayout->addWidget(btnDel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnOk = new QPushButton(ConfigEditDialog);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));

        horizontalLayout->addWidget(btnOk);

        btnCancel = new QPushButton(ConfigEditDialog);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        horizontalLayout->addWidget(btnCancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ConfigEditDialog);

        btnOk->setDefault(true);


        QMetaObject::connectSlotsByName(ConfigEditDialog);
    } // setupUi

    void retranslateUi(QDialog *ConfigEditDialog)
    {
        ConfigEditDialog->setWindowTitle(QCoreApplication::translate("ConfigEditDialog", "\347\274\226\350\276\221\351\205\215\347\275\256", nullptr));
        hint->setText(QCoreApplication::translate("ConfigEditDialog", "\345\217\214\345\207\273\345\215\225\345\205\203\346\240\274\345\217\257\347\233\264\346\216\245\347\274\226\350\276\221\343\200\202\347\202\271\344\275\215\345\220\215\347\247\260\344\270\216\345\257\204\345\255\230\345\231\250\345\234\260\345\235\200\351\203\275\344\270\215\350\203\275\351\207\215\345\244\215\343\200\202", nullptr));
        hint->setStyleSheet(QCoreApplication::translate("ConfigEditDialog", "color:#555;", nullptr));
        btnAdd->setText(QCoreApplication::translate("ConfigEditDialog", "\346\267\273\345\212\240\350\241\214", nullptr));
        btnDel->setText(QCoreApplication::translate("ConfigEditDialog", "\345\210\240\351\231\244\351\200\211\344\270\255\350\241\214", nullptr));
        btnOk->setText(QCoreApplication::translate("ConfigEditDialog", "\347\241\256\345\256\232", nullptr));
        btnCancel->setText(QCoreApplication::translate("ConfigEditDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfigEditDialog: public Ui_ConfigEditDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGEDITDIALOG_H
