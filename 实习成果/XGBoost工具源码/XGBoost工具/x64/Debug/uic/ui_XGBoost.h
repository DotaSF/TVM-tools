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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_XGBoostClass
{
public:
    QWidget *centralWidget;
    QPushButton *optimize;
    QPushButton *train;
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *train_path;
    QLineEdit *valid_path;
    QLabel *label_2;
    QLineEdit *train_rows;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *valid_rows;
    QLabel *label_5;
    QLineEdit *features_cols;
    QPushButton *select_train;
    QPushButton *select_valid;
    QLineEdit *model_path;
    QLabel *label_6;
    QTextBrowser *textBrowser;
    QLabel *label_7;
    QLabel *label_8;
    QGroupBox *groupBox_2;
    QLabel *label_10;
    QLabel *label_9;
    QLineEdit *min_child;
    QLineEdit *max_depth;
    QLabel *label_11;
    QLineEdit *gamma;
    QLabel *label_12;
    QLineEdit *subsample;
    QLineEdit *colsample;
    QLabel *label_13;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *XGBoostClass)
    {
        if (XGBoostClass->objectName().isEmpty())
            XGBoostClass->setObjectName(QString::fromUtf8("XGBoostClass"));
        XGBoostClass->setEnabled(true);
        XGBoostClass->resize(870, 550);
        centralWidget = new QWidget(XGBoostClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        optimize = new QPushButton(centralWidget);
        optimize->setObjectName(QString::fromUtf8("optimize"));
        optimize->setGeometry(QRect(690, 380, 111, 41));
        train = new QPushButton(centralWidget);
        train->setObjectName(QString::fromUtf8("train"));
        train->setGeometry(QRect(690, 427, 111, 41));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(500, 60, 331, 201));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(9, 40, 121, 20));
        label->setFrameShadow(QFrame::Plain);
        train_path = new QLineEdit(groupBox);
        train_path->setObjectName(QString::fromUtf8("train_path"));
        train_path->setGeometry(QRect(140, 35, 150, 30));
        valid_path = new QLineEdit(groupBox);
        valid_path->setObjectName(QString::fromUtf8("valid_path"));
        valid_path->setGeometry(QRect(140, 75, 150, 30));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(9, 80, 121, 20));
        label_2->setFrameShadow(QFrame::Plain);
        train_rows = new QLineEdit(groupBox);
        train_rows->setObjectName(QString::fromUtf8("train_rows"));
        train_rows->setGeometry(QRect(120, 120, 30, 30));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 130, 71, 20));
        label_3->setFrameShadow(QFrame::Plain);
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(40, 160, 81, 20));
        label_4->setFrameShadow(QFrame::Plain);
        valid_rows = new QLineEdit(groupBox);
        valid_rows->setObjectName(QString::fromUtf8("valid_rows"));
        valid_rows->setGeometry(QRect(120, 150, 30, 30));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(190, 140, 61, 16));
        label_5->setFrameShadow(QFrame::Plain);
        features_cols = new QLineEdit(groupBox);
        features_cols->setObjectName(QString::fromUtf8("features_cols"));
        features_cols->setGeometry(QRect(250, 130, 30, 30));
        select_train = new QPushButton(groupBox);
        select_train->setObjectName(QString::fromUtf8("select_train"));
        select_train->setGeometry(QRect(290, 35, 21, 28));
        select_valid = new QPushButton(groupBox);
        select_valid->setObjectName(QString::fromUtf8("select_valid"));
        select_valid->setGeometry(QRect(290, 75, 21, 28));
        model_path = new QLineEdit(centralWidget);
        model_path->setObjectName(QString::fromUtf8("model_path"));
        model_path->setGeometry(QRect(240, 430, 150, 30));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(150, 440, 81, 16));
        label_6->setFrameShadow(QFrame::Plain);
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(50, 120, 341, 251));
        textBrowser->setFrameShadow(QFrame::Plain);
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(20, 20, 141, 91));
        label_7->setFrameShadow(QFrame::Plain);
        label_7->setPixmap(QPixmap(QString::fromUtf8("logo.png")));
        label_7->setScaledContents(true);
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(200, 30, 271, 51));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(19);
        font.setBold(false);
        font.setWeight(50);
        label_8->setFont(font);
        label_8->setFrameShadow(QFrame::Plain);
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(500, 290, 151, 191));
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 40, 80, 20));
        label_10->setFrameShadow(QFrame::Plain);
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 70, 80, 20));
        label_9->setFrameShadow(QFrame::Plain);
        min_child = new QLineEdit(groupBox_2);
        min_child->setObjectName(QString::fromUtf8("min_child"));
        min_child->setGeometry(QRect(100, 60, 30, 30));
        max_depth = new QLineEdit(groupBox_2);
        max_depth->setObjectName(QString::fromUtf8("max_depth"));
        max_depth->setGeometry(QRect(100, 30, 30, 30));
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(40, 100, 40, 20));
        label_11->setFrameShadow(QFrame::Plain);
        gamma = new QLineEdit(groupBox_2);
        gamma->setObjectName(QString::fromUtf8("gamma"));
        gamma->setGeometry(QRect(100, 90, 30, 30));
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 130, 80, 20));
        label_12->setFrameShadow(QFrame::Plain);
        subsample = new QLineEdit(groupBox_2);
        subsample->setObjectName(QString::fromUtf8("subsample"));
        subsample->setGeometry(QRect(100, 120, 30, 30));
        colsample = new QLineEdit(groupBox_2);
        colsample->setObjectName(QString::fromUtf8("colsample"));
        colsample->setGeometry(QRect(100, 150, 30, 30));
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(10, 160, 80, 20));
        label_13->setFrameShadow(QFrame::Plain);
        XGBoostClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(XGBoostClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 870, 26));
        XGBoostClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(XGBoostClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        XGBoostClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(XGBoostClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        XGBoostClass->setStatusBar(statusBar);

        retranslateUi(XGBoostClass);

        QMetaObject::connectSlotsByName(XGBoostClass);
    } // setupUi

    void retranslateUi(QMainWindow *XGBoostClass)
    {
        XGBoostClass->setWindowTitle(QCoreApplication::translate("XGBoostClass", "XGBoost", nullptr));
        optimize->setText(QCoreApplication::translate("XGBoostClass", "XGBoost\350\260\203\345\217\202", nullptr));
        train->setText(QCoreApplication::translate("XGBoostClass", "XGBoost\350\256\255\347\273\203", nullptr));
        groupBox->setTitle(QCoreApplication::translate("XGBoostClass", "\350\260\203\345\217\202\350\256\276\347\275\256", nullptr));
        label->setText(QCoreApplication::translate("XGBoostClass", "(\350\213\261\346\226\207)\350\256\255\347\273\203\351\233\206\350\267\257\345\276\204\357\274\232", nullptr));
        train_path->setText(QCoreApplication::translate("XGBoostClass", "./train.txt", nullptr));
        valid_path->setText(QCoreApplication::translate("XGBoostClass", "./valid.txt", nullptr));
        label_2->setText(QCoreApplication::translate("XGBoostClass", "(\350\213\261\346\226\207)\351\252\214\350\257\201\351\233\206\350\267\257\345\276\204\357\274\232", nullptr));
        train_rows->setText(QCoreApplication::translate("XGBoostClass", "0", nullptr));
        label_3->setText(QCoreApplication::translate("XGBoostClass", "\350\256\255\347\273\203\350\241\214\346\225\260\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("XGBoostClass", "\351\252\214\350\257\201\350\241\214\346\225\260\357\274\232", nullptr));
        valid_rows->setText(QCoreApplication::translate("XGBoostClass", "0", nullptr));
        label_5->setText(QCoreApplication::translate("XGBoostClass", "\347\211\271\345\276\201\346\225\260\357\274\232", nullptr));
        features_cols->setText(QCoreApplication::translate("XGBoostClass", "21", nullptr));
        select_train->setText(QCoreApplication::translate("XGBoostClass", "\342\200\246", nullptr));
        select_valid->setText(QCoreApplication::translate("XGBoostClass", "\342\200\246", nullptr));
        label_6->setText(QCoreApplication::translate("XGBoostClass", "\350\276\223\345\207\272\346\250\241\345\236\213\357\274\232", nullptr));
        label_7->setText(QString());
        label_8->setText(QCoreApplication::translate("XGBoostClass", "XGBoost\350\256\255\347\273\203\345\267\245\345\205\267", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("XGBoostClass", "\350\256\255\347\273\203\345\217\202\346\225\260", nullptr));
        label_10->setText(QCoreApplication::translate("XGBoostClass", "max_depth", nullptr));
        label_9->setText(QCoreApplication::translate("XGBoostClass", "min_child", nullptr));
        min_child->setText(QCoreApplication::translate("XGBoostClass", "1", nullptr));
        max_depth->setText(QCoreApplication::translate("XGBoostClass", "5", nullptr));
        label_11->setText(QCoreApplication::translate("XGBoostClass", "gamma", nullptr));
        gamma->setText(QCoreApplication::translate("XGBoostClass", "0.0", nullptr));
        label_12->setText(QCoreApplication::translate("XGBoostClass", "subsample", nullptr));
        subsample->setText(QCoreApplication::translate("XGBoostClass", "0.8", nullptr));
        colsample->setText(QCoreApplication::translate("XGBoostClass", "0.8", nullptr));
        label_13->setText(QCoreApplication::translate("XGBoostClass", "colsample", nullptr));
    } // retranslateUi

};

namespace Ui {
    class XGBoostClass: public Ui_XGBoostClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_XGBOOST_H
