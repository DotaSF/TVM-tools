#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_XGBoost.h"

class XGBoost : public QMainWindow
{
    Q_OBJECT

public:
    XGBoost(QWidget *parent = Q_NULLPTR);

private:
    Ui::XGBoostClass ui;
private slots://声明所有槽函数，自定义信号怎么办，例如某个变量达到一定值，触发槽函数
	void on_optimize_clicked();
	void on_train_clicked();
	void on_select_train_clicked();
	void on_select_valid_clicked();
	//打开设备
	//void on_train_clicked();//关闭设备
	//void uqdateUI();//更新UI界面
	//void destorywin();
};
