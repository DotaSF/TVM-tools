/********************************************************************************
** Form generated from reading UI file 'XGBoost.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_XGBOOST_H
#define UI_XGBOOST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XGBoostClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *XGBoostClass)
    {
        if (XGBoostClass->objectName().isEmpty())
            XGBoostClass->setObjectName(QString::fromUtf8("XGBoostClass"));
        XGBoostClass->resize(600, 400);
        menuBar = new QMenuBar(XGBoostClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        XGBoostClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(XGBoostClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        XGBoostClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(XGBoostClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        XGBoostClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(XGBoostClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        XGBoostClass->setStatusBar(statusBar);

        retranslateUi(XGBoostClass);

        QMetaObject::connectSlotsByName(XGBoostClass);
    } // setupUi

    void retranslateUi(QMainWindow *XGBoostClass)
    {
        XGBoostClass->setWindowTitle(QCoreApplication::translate("XGBoostClass", "XGBoost", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XGBoostClass: public Ui_XGBoostClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XGBOOST_H
