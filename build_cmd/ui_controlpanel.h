/********************************************************************************
** Form generated from reading UI file 'controlpanel.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROLPANEL_H
#define UI_CONTROLPANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlPanel
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *title;
    QGridLayout *grid;
    QLabel *lblPt;
    QComboBox *comboPoint;
    QLabel *lblVal;
    QDoubleSpinBox *spinValue;
    QPushButton *btnSend;
    QLabel *labelStatus;
    QSpacerItem *vspacer;

    void setupUi(QWidget *ControlPanel)
    {
        if (ControlPanel->objectName().isEmpty())
            ControlPanel->setObjectName(QString::fromUtf8("ControlPanel"));
        ControlPanel->resize(500, 200);
        verticalLayout = new QVBoxLayout(ControlPanel);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        title = new QLabel(ControlPanel);
        title->setObjectName(QString::fromUtf8("title"));

        verticalLayout->addWidget(title);

        grid = new QGridLayout();
        grid->setObjectName(QString::fromUtf8("grid"));
        lblPt = new QLabel(ControlPanel);
        lblPt->setObjectName(QString::fromUtf8("lblPt"));

        grid->addWidget(lblPt, 0, 0, 1, 1);

        comboPoint = new QComboBox(ControlPanel);
        comboPoint->setObjectName(QString::fromUtf8("comboPoint"));
        comboPoint->setMinimumWidth(180);

        grid->addWidget(comboPoint, 0, 1, 1, 1);

        lblVal = new QLabel(ControlPanel);
        lblVal->setObjectName(QString::fromUtf8("lblVal"));

        grid->addWidget(lblVal, 1, 0, 1, 1);

        spinValue = new QDoubleSpinBox(ControlPanel);
        spinValue->setObjectName(QString::fromUtf8("spinValue"));
        spinValue->setMinimumWidth(180);

        grid->addWidget(spinValue, 1, 1, 1, 1);

        btnSend = new QPushButton(ControlPanel);
        btnSend->setObjectName(QString::fromUtf8("btnSend"));

        grid->addWidget(btnSend, 2, 1, 1, 1);


        verticalLayout->addLayout(grid);

        labelStatus = new QLabel(ControlPanel);
        labelStatus->setObjectName(QString::fromUtf8("labelStatus"));

        verticalLayout->addWidget(labelStatus);

        vspacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(vspacer);


        retranslateUi(ControlPanel);

        QMetaObject::connectSlotsByName(ControlPanel);
    } // setupUi

    void retranslateUi(QWidget *ControlPanel)
    {
        title->setText(QCoreApplication::translate("ControlPanel", "\346\225\260\346\215\256\344\270\213\345\217\221", nullptr));
        title->setStyleSheet(QCoreApplication::translate("ControlPanel", "font-weight:bold; font-size:13px; padding:2px;", nullptr));
        lblPt->setText(QCoreApplication::translate("ControlPanel", "\347\202\271\344\275\215", nullptr));
        lblVal->setText(QCoreApplication::translate("ControlPanel", "\345\200\274", nullptr));
        btnSend->setText(QCoreApplication::translate("ControlPanel", "\344\270\213\345\217\221", nullptr));
        labelStatus->setText(QCoreApplication::translate("ControlPanel", "\345\260\261\347\273\252", nullptr));
        labelStatus->setStyleSheet(QCoreApplication::translate("ControlPanel", "color:#444; padding:2px;", nullptr));
        (void)ControlPanel;
    } // retranslateUi

};

namespace Ui {
    class ControlPanel: public Ui_ControlPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROLPANEL_H
