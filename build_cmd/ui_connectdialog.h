/********************************************************************************
** Form generated from reading UI file 'connectdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTDIALOG_H
#define UI_CONNECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConnectDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *labelIp;
    QLineEdit *lineIp;
    QLabel *labelPort;
    QSpinBox *spinPort;
    QLabel *labelSlave;
    QSpinBox *spinSlaveId;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ConnectDialog)
    {
        if (ConnectDialog->objectName().isEmpty())
            ConnectDialog->setObjectName(QString::fromUtf8("ConnectDialog"));
        ConnectDialog->resize(340, 200);
        verticalLayout = new QVBoxLayout(ConnectDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        labelIp = new QLabel(ConnectDialog);
        labelIp->setObjectName(QString::fromUtf8("labelIp"));

        formLayout->setWidget(0, QFormLayout::LabelRole, labelIp);

        lineIp = new QLineEdit(ConnectDialog);
        lineIp->setObjectName(QString::fromUtf8("lineIp"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineIp);

        labelPort = new QLabel(ConnectDialog);
        labelPort->setObjectName(QString::fromUtf8("labelPort"));

        formLayout->setWidget(1, QFormLayout::LabelRole, labelPort);

        spinPort = new QSpinBox(ConnectDialog);
        spinPort->setObjectName(QString::fromUtf8("spinPort"));
        spinPort->setMinimum(1);
        spinPort->setMaximum(65535);
        spinPort->setValue(502);

        formLayout->setWidget(1, QFormLayout::FieldRole, spinPort);

        labelSlave = new QLabel(ConnectDialog);
        labelSlave->setObjectName(QString::fromUtf8("labelSlave"));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelSlave);

        spinSlaveId = new QSpinBox(ConnectDialog);
        spinSlaveId->setObjectName(QString::fromUtf8("spinSlaveId"));
        spinSlaveId->setMinimum(0);
        spinSlaveId->setMaximum(255);
        spinSlaveId->setValue(1);

        formLayout->setWidget(2, QFormLayout::FieldRole, spinSlaveId);


        verticalLayout->addLayout(formLayout);

        buttonBox = new QDialogButtonBox(ConnectDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ConnectDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ConnectDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ConnectDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ConnectDialog);
    } // setupUi

    void retranslateUi(QDialog *ConnectDialog)
    {
        ConnectDialog->setWindowTitle(QCoreApplication::translate("ConnectDialog", "\350\277\236\346\216\245 PLC", nullptr));
        labelIp->setText(QCoreApplication::translate("ConnectDialog", "IP \345\234\260\345\235\200", nullptr));
        lineIp->setText(QCoreApplication::translate("ConnectDialog", "127.0.0.1", nullptr));
        labelPort->setText(QCoreApplication::translate("ConnectDialog", "\347\253\257\345\217\243", nullptr));
        labelSlave->setText(QCoreApplication::translate("ConnectDialog", "\344\273\216\347\253\231 ID", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConnectDialog: public Ui_ConnectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTDIALOG_H
