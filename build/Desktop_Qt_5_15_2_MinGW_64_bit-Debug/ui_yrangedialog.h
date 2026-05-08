/********************************************************************************
** Form generated from reading UI file 'yrangedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_YRANGEDIALOG_H
#define UI_YRANGEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_YRangeDialog
{
public:
    QVBoxLayout *verticalLayout;
    QCheckBox *checkAuto;
    QFormLayout *formLayout;
    QLabel *labelMin;
    QDoubleSpinBox *spinMin;
    QLabel *labelMax;
    QDoubleSpinBox *spinMax;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *YRangeDialog)
    {
        if (YRangeDialog->objectName().isEmpty())
            YRangeDialog->setObjectName(QString::fromUtf8("YRangeDialog"));
        YRangeDialog->resize(340, 180);
        verticalLayout = new QVBoxLayout(YRangeDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        checkAuto = new QCheckBox(YRangeDialog);
        checkAuto->setObjectName(QString::fromUtf8("checkAuto"));

        verticalLayout->addWidget(checkAuto);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        labelMin = new QLabel(YRangeDialog);
        labelMin->setObjectName(QString::fromUtf8("labelMin"));

        formLayout->setWidget(0, QFormLayout::LabelRole, labelMin);

        spinMin = new QDoubleSpinBox(YRangeDialog);
        spinMin->setObjectName(QString::fromUtf8("spinMin"));

        formLayout->setWidget(0, QFormLayout::FieldRole, spinMin);

        labelMax = new QLabel(YRangeDialog);
        labelMax->setObjectName(QString::fromUtf8("labelMax"));

        formLayout->setWidget(1, QFormLayout::LabelRole, labelMax);

        spinMax = new QDoubleSpinBox(YRangeDialog);
        spinMax->setObjectName(QString::fromUtf8("spinMax"));

        formLayout->setWidget(1, QFormLayout::FieldRole, spinMax);


        verticalLayout->addLayout(formLayout);

        buttonBox = new QDialogButtonBox(YRangeDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(YRangeDialog);

        QMetaObject::connectSlotsByName(YRangeDialog);
    } // setupUi

    void retranslateUi(QDialog *YRangeDialog)
    {
        YRangeDialog->setWindowTitle(QCoreApplication::translate("YRangeDialog", "\350\256\276\347\275\256 Y \350\275\264\350\214\203\345\233\264", nullptr));
        checkAuto->setText(QCoreApplication::translate("YRangeDialog", "\350\207\252\345\212\250\347\274\251\346\224\276\357\274\210\346\214\211\346\225\260\346\215\256\350\214\203\345\233\264\350\260\203\346\225\264\357\274\211", nullptr));
        labelMin->setText(QCoreApplication::translate("YRangeDialog", "\346\234\200\345\260\217\345\200\274", nullptr));
        labelMax->setText(QCoreApplication::translate("YRangeDialog", "\346\234\200\345\244\247\345\200\274", nullptr));
    } // retranslateUi

};

namespace Ui {
    class YRangeDialog: public Ui_YRangeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_YRANGEDIALOG_H
