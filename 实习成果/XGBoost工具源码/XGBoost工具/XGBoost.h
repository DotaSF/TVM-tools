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
private slots://�������вۺ������Զ����ź���ô�죬����ĳ�������ﵽһ��ֵ�������ۺ���
	void on_optimize_clicked();
	void on_train_clicked();
	void on_select_train_clicked();
	void on_select_valid_clicked();
	//���豸
	//void on_train_clicked();//�ر��豸
	//void uqdateUI();//����UI����
	//void destorywin();
};
