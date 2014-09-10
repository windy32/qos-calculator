/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created: Wed Sep 10 19:48:29 2014
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QHBoxLayout *horizontalLayout;
    QTabWidget *tab;
    QWidget *tabCalculator;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *hLayout1;
    QLabel *lblBandwidth;
    QDoubleSpinBox *spnBandwidth;
    QLabel *lblUnit;
    QLabel *lblDescription;
    QLabel *lblTokenRate;
    QHBoxLayout *hLayout2;
    QLabel *lblBufferSize;
    QPushButton *btnCalculate;
    QWidget *tabGenerator;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *hLayout3;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout5;
    QLabel *lblAddress;
    QLineEdit *txtAddress;
    QPushButton *btnAdd;
    QPlainTextEdit *txtScript;
    QListWidget *lstAddress;
    QHBoxLayout *hLayout4;
    QPushButton *btnGenerate;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnRemove;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(607, 380);
        horizontalLayout = new QHBoxLayout(Dialog);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        tab = new QTabWidget(Dialog);
        tab->setObjectName(QString::fromUtf8("tab"));
        tabCalculator = new QWidget();
        tabCalculator->setObjectName(QString::fromUtf8("tabCalculator"));
        verticalLayout = new QVBoxLayout(tabCalculator);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        hLayout1 = new QHBoxLayout();
        hLayout1->setSpacing(6);
        hLayout1->setObjectName(QString::fromUtf8("hLayout1"));
        lblBandwidth = new QLabel(tabCalculator);
        lblBandwidth->setObjectName(QString::fromUtf8("lblBandwidth"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lblBandwidth->sizePolicy().hasHeightForWidth());
        lblBandwidth->setSizePolicy(sizePolicy);

        hLayout1->addWidget(lblBandwidth);

        spnBandwidth = new QDoubleSpinBox(tabCalculator);
        spnBandwidth->setObjectName(QString::fromUtf8("spnBandwidth"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(spnBandwidth->sizePolicy().hasHeightForWidth());
        spnBandwidth->setSizePolicy(sizePolicy1);
        spnBandwidth->setMinimumSize(QSize(80, 0));
        spnBandwidth->setMinimum(1);
        spnBandwidth->setMaximum(100);
        spnBandwidth->setSingleStep(0.01);
        spnBandwidth->setValue(8);

        hLayout1->addWidget(spnBandwidth);

        lblUnit = new QLabel(tabCalculator);
        lblUnit->setObjectName(QString::fromUtf8("lblUnit"));

        hLayout1->addWidget(lblUnit);


        verticalLayout->addLayout(hLayout1);

        lblDescription = new QLabel(tabCalculator);
        lblDescription->setObjectName(QString::fromUtf8("lblDescription"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lblDescription->sizePolicy().hasHeightForWidth());
        lblDescription->setSizePolicy(sizePolicy2);
        lblDescription->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout->addWidget(lblDescription);

        lblTokenRate = new QLabel(tabCalculator);
        lblTokenRate->setObjectName(QString::fromUtf8("lblTokenRate"));

        verticalLayout->addWidget(lblTokenRate);

        hLayout2 = new QHBoxLayout();
        hLayout2->setSpacing(6);
        hLayout2->setObjectName(QString::fromUtf8("hLayout2"));
        lblBufferSize = new QLabel(tabCalculator);
        lblBufferSize->setObjectName(QString::fromUtf8("lblBufferSize"));
        sizePolicy.setHeightForWidth(lblBufferSize->sizePolicy().hasHeightForWidth());
        lblBufferSize->setSizePolicy(sizePolicy);

        hLayout2->addWidget(lblBufferSize);

        btnCalculate = new QPushButton(tabCalculator);
        btnCalculate->setObjectName(QString::fromUtf8("btnCalculate"));
        btnCalculate->setMinimumSize(QSize(120, 0));

        hLayout2->addWidget(btnCalculate);


        verticalLayout->addLayout(hLayout2);

        tab->addTab(tabCalculator, QString());
        tabGenerator = new QWidget();
        tabGenerator->setObjectName(QString::fromUtf8("tabGenerator"));
        verticalLayout_2 = new QVBoxLayout(tabGenerator);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        hLayout3 = new QHBoxLayout();
        hLayout3->setSpacing(6);
        hLayout3->setObjectName(QString::fromUtf8("hLayout3"));
        hLayout3->setSizeConstraint(QLayout::SetDefaultConstraint);
        vLayout = new QVBoxLayout();
        vLayout->setSpacing(6);
        vLayout->setObjectName(QString::fromUtf8("vLayout"));
        hLayout5 = new QHBoxLayout();
        hLayout5->setSpacing(6);
        hLayout5->setObjectName(QString::fromUtf8("hLayout5"));
        lblAddress = new QLabel(tabGenerator);
        lblAddress->setObjectName(QString::fromUtf8("lblAddress"));

        hLayout5->addWidget(lblAddress);

        txtAddress = new QLineEdit(tabGenerator);
        txtAddress->setObjectName(QString::fromUtf8("txtAddress"));
        txtAddress->setMaxLength(15);

        hLayout5->addWidget(txtAddress);

        btnAdd = new QPushButton(tabGenerator);
        btnAdd->setObjectName(QString::fromUtf8("btnAdd"));

        hLayout5->addWidget(btnAdd);


        vLayout->addLayout(hLayout5);

        txtScript = new QPlainTextEdit(tabGenerator);
        txtScript->setObjectName(QString::fromUtf8("txtScript"));
        txtScript->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        txtScript->setLineWrapMode(QPlainTextEdit::NoWrap);

        vLayout->addWidget(txtScript);


        hLayout3->addLayout(vLayout);

        lstAddress = new QListWidget(tabGenerator);
        lstAddress->setObjectName(QString::fromUtf8("lstAddress"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(lstAddress->sizePolicy().hasHeightForWidth());
        lstAddress->setSizePolicy(sizePolicy3);
        lstAddress->setMinimumSize(QSize(150, 0));
        lstAddress->setMaximumSize(QSize(150, 16777215));
        lstAddress->setEditTriggers(QAbstractItemView::NoEditTriggers);

        hLayout3->addWidget(lstAddress);


        verticalLayout_2->addLayout(hLayout3);

        hLayout4 = new QHBoxLayout();
        hLayout4->setSpacing(6);
        hLayout4->setObjectName(QString::fromUtf8("hLayout4"));
        btnGenerate = new QPushButton(tabGenerator);
        btnGenerate->setObjectName(QString::fromUtf8("btnGenerate"));
        btnGenerate->setEnabled(false);
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(btnGenerate->sizePolicy().hasHeightForWidth());
        btnGenerate->setSizePolicy(sizePolicy4);
        btnGenerate->setMinimumSize(QSize(140, 0));

        hLayout4->addWidget(btnGenerate);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hLayout4->addItem(horizontalSpacer);

        btnRemove = new QPushButton(tabGenerator);
        btnRemove->setObjectName(QString::fromUtf8("btnRemove"));
        btnRemove->setEnabled(false);
        btnRemove->setMinimumSize(QSize(100, 0));

        hLayout4->addWidget(btnRemove);


        verticalLayout_2->addLayout(hLayout4);

        tab->addTab(tabGenerator, QString());

        horizontalLayout->addWidget(tab);


        retranslateUi(Dialog);

        tab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "QoS Calculator", 0, QApplication::UnicodeUTF8));
        lblBandwidth->setText(QApplication::translate("Dialog", "Downstream bandwidth:", 0, QApplication::UnicodeUTF8));
        lblUnit->setText(QApplication::translate("Dialog", "Mbps", 0, QApplication::UnicodeUTF8));
        lblDescription->setText(QApplication::translate("Dialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600;\">Enter how fast you can download.</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Use an online broadband testing service (e.g., speedtest.net) to get your real speed.</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">You can measure in different periods in a day, and then input the lowest value.</p></body></html>", 0, QApplication::UnicodeUTF8));
        lblTokenRate->setText(QApplication::translate("Dialog", "Token rate: - Mbps", 0, QApplication::UnicodeUTF8));
        lblBufferSize->setText(QApplication::translate("Dialog", "Buffer size: - Bytes", 0, QApplication::UnicodeUTF8));
        btnCalculate->setText(QApplication::translate("Dialog", "Calculate", 0, QApplication::UnicodeUTF8));
        tab->setTabText(tab->indexOf(tabCalculator), QApplication::translate("Dialog", "Calculator", 0, QApplication::UnicodeUTF8));
        lblAddress->setText(QApplication::translate("Dialog", "IP Address", 0, QApplication::UnicodeUTF8));
        btnAdd->setText(QApplication::translate("Dialog", "Add", 0, QApplication::UnicodeUTF8));
        btnGenerate->setText(QApplication::translate("Dialog", "Generate Script", 0, QApplication::UnicodeUTF8));
        btnRemove->setText(QApplication::translate("Dialog", "Remove", 0, QApplication::UnicodeUTF8));
        tab->setTabText(tab->indexOf(tabGenerator), QApplication::translate("Dialog", "Script Generator", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
