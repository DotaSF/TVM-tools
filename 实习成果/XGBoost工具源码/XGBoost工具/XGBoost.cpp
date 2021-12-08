#include "XGBoost.h"
#include"ui_XGBoost.h"
#include <QtWidgets/QApplication>
#include <Windows.h>
#include <QtGui>
#include<QMessageBox>
#include<iostream>
#include<xgboost/c_api.h>
//���߳�
//#include <boost/thread/thread.hpp>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <QFileDialog>

using namespace std;

void load_valid_data(float* valid_data, float* valid_labels);
float validate(int iter, float* valid_data, float* valid_labels, QTextBrowser* textBrowser);
float RMSE(const float* Data, float* Data2, int Num);
void load_train_data(float* train_data, float* train_labels);


XGBoost::XGBoost(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

string filepath;//��ȡѵ���ļ���ַ
string valid_path;//��ȡ��֤�ļ���ַ
int train_rows;
int valid_rows;
int features_cols;
void XGBoost::on_optimize_clicked()//�Զ��Ż�
{
	//������Ҫ��ȡ����
	QString train_path_ = ui.train_path->text(); filepath = train_path_.toStdString();//��ȡѵ���ļ���ַ
	QString valid_path_ = ui.valid_path->text(); valid_path = valid_path_.toStdString();//��ȡ��֤�ļ���ַ
	QString train_rows_ = ui.train_rows->text(); train_rows = train_rows_.toInt();
	QString valid_rows_ = ui.valid_rows->text(); valid_rows = valid_rows_.toInt();
	QString features_cols_ = ui.features_cols->text(); features_cols = features_cols_.toInt();

	const int num_booster = 200;//��������
	float* train_labels = new float[train_rows];//ѵ����ǩ����
	float* train_data = new float[train_rows * features_cols];//ѵ������
	float* valid_labels = new float[valid_rows];//��֤��ǩ����
	float* valid_data = new float[valid_rows * features_cols];//��֤����
	load_train_data((float*)train_data, (float*)train_labels);//����ѵ������
	load_valid_data((float*)valid_data, (float*)valid_labels);//������֤����

	//ת����DMatrix���ݸ�ʽ
	DMatrixHandle h_train[1];
	XGDMatrixCreateFromMat((float*)train_data, train_rows, features_cols, -1, &h_train[0]);
	//���ر�ǩ
	XGDMatrixSetFloatInfo(h_train[0], "label", train_labels, train_rows);//ѵ����Ӧ�ı�ǩ��ΪDMatrix����ǩ����
	//����ʵ�������ò���
	BoosterHandle h_booster;
	XGBoosterCreate(h_train, 1, &h_booster);
	XGBoosterSetParam(h_booster, "booster", "gbtree");//ģ�͵���ʽ
	XGBoosterSetParam(h_booster, "objective", "reg:squarederror");//Ŀ�꺯��
	XGBoosterSetParam(h_booster, "max_depth", "5");
	XGBoosterSetParam(h_booster, "eta", "0.1");
	XGBoosterSetParam(h_booster, "min_child_weight", "1");
	XGBoosterSetParam(h_booster, "subsample", "0.8");
	XGBoosterSetParam(h_booster, "colsample_bytree", "0.8");
	XGBoosterSetParam(h_booster, "gamma", "0");
	//ִ�е�������������������������
	float MIN = FLT_MAX;//����RMSE����Сֵ
	int flag_boosters = 1;////////////////////////////////////////////////////////////////////////////////////////
	for (int iter = 1; iter <= num_booster; iter++)
	{
		XGBoosterUpdateOneIter(h_booster, iter, h_train[0]);
		if (iter > 0 && (iter % 4 == 0))//4������֤һ��
		{
			string fname = "./" + to_string(iter) + "xgb.model";
			XGBoosterSaveModel(h_booster, fname.c_str());
			float rmse = validate(iter, (float*)valid_data, (float*)valid_labels, ui.textBrowser);
			if (rmse < MIN)//��ǰ���������и�Сrmse
			{
				MIN = rmse;
				flag_boosters = iter;//��¼��Сrmse��Ӧ��������
			}
		}
	}
	XGBoosterFree(h_booster);//�ͷž��

	string message = "iterations:  " + to_string(flag_boosters) + "		RMSE:  " + to_string(MIN);
	ui.textBrowser->append(QString(QString::fromLocal8Bit(message.c_str())));
	//����Ѱ����boosters������������max_depth �� min_child_weight���ţ��ֱ�������������Ⱥ�Ҷ�ӽ������������
	//������Χ��max_depth->[3,10]��min_child_weight->[1,6]
	int depth1 = 3, depth2 = 10, weight1 = 1, weight2 = 6;
	int max_depth = 3, min_child_weight = 1;//////////////////////////////////////////////////////////////////////////////
	MIN = FLT_MAX;//����RMSE����Сֵ
	int zuo1 = depth1;
	for (; zuo1 <= depth2; zuo1++)//դ������
	{
		int zuo2 = weight1;
		for (; zuo2 <= weight2; zuo2++)
		{
			BoosterHandle h_booster;
			XGBoosterCreate(h_train, 1, &h_booster);
			string canshu = to_string(zuo1);
			string canshu2 = to_string(zuo2);
			XGBoosterSetParam(h_booster, "max_depth", canshu.c_str());
			XGBoosterSetParam(h_booster, "min_child_weight", canshu2.c_str());
			XGBoosterSetParam(h_booster, "booster", "gbtree");//ģ�͵���ʽ
			XGBoosterSetParam(h_booster, "objective", "reg:squarederror");//Ŀ�꺯��
			XGBoosterSetParam(h_booster, "eta", "0.1");
			XGBoosterSetParam(h_booster, "subsample", "0.8");
			XGBoosterSetParam(h_booster, "colsample_bytree", "0.8");
			XGBoosterSetParam(h_booster, "gamma", "0");

			//ִ�е���������������֮ǰ���Գ�����
			for (int iter = 1; iter <= flag_boosters; iter++)
			{
				XGBoosterUpdateOneIter(h_booster, iter, h_train[0]);
			}
			cout << canshu << "    " << canshu2 << "    ";
			string fname = "./" + to_string(flag_boosters) + "xgb.model";
			XGBoosterSaveModel(h_booster, fname.c_str());

			float rmse = validate(flag_boosters, (float*)valid_data, (float*)valid_labels, ui.textBrowser);
			if (rmse < MIN)//��ǰ���������и�Сrmse
			{
				MIN = rmse;
				max_depth = zuo1;
				min_child_weight = zuo2;
			}
			//�ͷ����ݽṹ
			XGBoosterFree(h_booster);
		}
	}
	string message1 = "max_depth: " + to_string(max_depth) + "  min_child_weight:" + to_string(min_child_weight);
	ui.textBrowser->append(QString(QString::fromLocal8Bit(message1.c_str())));

	//�������max_depth �� min_child_weight�ĵ��ţ�����gamma�ĵ��ţ����򻯲���
	//������Χ��gamma->[0.0,1.0]�������Χ������[0,������]�����������ֵԽ���㷨Խ����
	float gamma1 = 0.0, gamma2 = 1.0;//����ֵ
	float gamma = 0.0;//////////////////////////////////////////////////////////////////////////////////////////////////
	MIN = FLT_MAX;//����RMSE����Сֵ
	float left1 = gamma1;
	for (; left1 <= gamma2; left1 += 0.1)//դ������
	{
		string canshu = to_string(left1);
		BoosterHandle h_booster;
		XGBoosterCreate(h_train, 1, &h_booster);
		XGBoosterSetParam(h_booster, "booster", "gbtree");//ģ�͵���ʽ
		XGBoosterSetParam(h_booster, "objective", "reg:squarederror");//Ŀ�꺯��
		XGBoosterSetParam(h_booster, "max_depth", to_string(max_depth).c_str());
		XGBoosterSetParam(h_booster, "eta", "0.1");
		XGBoosterSetParam(h_booster, "min_child_weight", to_string(min_child_weight).c_str());
		XGBoosterSetParam(h_booster, "subsample", "0.8");
		XGBoosterSetParam(h_booster, "colsample_bytree", "0.8");
		XGBoosterSetParam(h_booster, "gamma", canshu.c_str());

		//ִ�е���������������֮ǰ���Գ�����
		for (int iter = 1; iter <= flag_boosters; iter++)
		{
			XGBoosterUpdateOneIter(h_booster, iter, h_train[0]);
		}
		cout << canshu << "    ";
		string fname = "./" + to_string(flag_boosters) + "xgb.model";
		XGBoosterSaveModel(h_booster, fname.c_str());
		float rmse = validate(flag_boosters, (float*)valid_data, (float*)valid_labels, ui.textBrowser);
		if (rmse < MIN)//��ǰ���������и�Сrmse
		{
			MIN = rmse;
			gamma = left1;
		}
		//�ͷ����ݽṹ
		XGBoosterFree(h_booster);
	}
	string message2 = "gamma:  " + to_string(gamma);
	ui.textBrowser->append(QString(QString::fromLocal8Bit(message2.c_str())));


	//�������gamma���Σ���������subsample �� colsample_bytree���š�subsample���ƶ���ÿ������ѵ��������������ı�����colsample_bytree����ÿ�����������������ռ��
	float subsample1 = 0.5, subsample2 = 1.0, colsample_bytree1 = 0.5, colsample_bytree2 = 1.0;
	float subsample = 0.5, colsample_bytree = 0.5;//////////////////////////////////////////////////////////////////////////////
	MIN = FLT_MAX;//����RMSE����Сֵ
	left1 = subsample1;
	for (; left1 <= subsample2; left1 += 0.05)//դ������
	{
		float left2 = colsample_bytree1;
		for (; left2 <= colsample_bytree2; left2 += 0.05)
		{
			string canshu = to_string(left1);
			string canshu2 = to_string(left2);
			BoosterHandle h_booster;
			XGBoosterCreate(h_train, 1, &h_booster);
			XGBoosterSetParam(h_booster, "booster", "gbtree");//ģ�͵���ʽ
			XGBoosterSetParam(h_booster, "objective", "reg:squarederror");//Ŀ�꺯��
			XGBoosterSetParam(h_booster, "max_depth", to_string(max_depth).c_str());
			XGBoosterSetParam(h_booster, "eta", "0.1");
			XGBoosterSetParam(h_booster, "min_child_weight", to_string(min_child_weight).c_str());
			XGBoosterSetParam(h_booster, "subsample", canshu.c_str());
			XGBoosterSetParam(h_booster, "colsample_bytree", canshu2.c_str());
			XGBoosterSetParam(h_booster, "gamma", to_string(gamma).c_str());

			for (int iter = 1; iter <= flag_boosters; iter++)
			{
				XGBoosterUpdateOneIter(h_booster, iter, h_train[0]);
			}
			cout << canshu << "    " << canshu2 << "    ";
			string fname = "./" + to_string(flag_boosters) + "xgb.model";
			XGBoosterSaveModel(h_booster, fname.c_str());
			float rmse = validate(flag_boosters, (float*)valid_data, (float*)valid_labels, ui.textBrowser);
			if (rmse < MIN)//��ǰ���������и�Сrmse
			{
				MIN = rmse;
				subsample = left1;
				colsample_bytree = left2;
			}
			//�ͷ����ݽṹ
			XGBoosterFree(h_booster);
		}
	}
	string message3 = "subsample:  " + to_string(subsample) + "	colsample_bytree:  " + to_string(colsample_bytree);
	ui.textBrowser->append(QString(QString::fromLocal8Bit(message1.c_str())));
	ui.textBrowser->append(QString(QString::fromLocal8Bit(message2.c_str())));
	ui.textBrowser->append(QString(QString::fromLocal8Bit(message3.c_str())));

	//�ͷ��ڴ�
	delete[]train_labels;
	delete[]train_data;
	delete[]valid_labels;
	delete[]valid_data;
	XGDMatrixFree(h_train[0]);
	return;
}
void XGBoost::on_train_clicked()//ѵ��
{
	QString train_path_ = ui.train_path->text(); filepath = train_path_.toStdString();//��ȡѵ���ļ���ַ
	QString valid_path_ = ui.valid_path->text(); valid_path = valid_path_.toStdString();//��ȡ��֤�ļ���ַ
	QString train_rows_ = ui.train_rows->text(); train_rows = train_rows_.toInt();
	QString valid_rows_ = ui.valid_rows->text(); valid_rows = valid_rows_.toInt();
	QString features_cols_ = ui.features_cols->text(); features_cols = features_cols_.toInt();
	const int num_booster = 200;//��������

	float* train_labels = new float[train_rows];//ѵ����ǩ����
	float* train_data = new float[train_rows* features_cols];//ѵ������
	float* valid_labels = new float[valid_rows];//��֤��ǩ����
	float* valid_data = new float[valid_rows * features_cols];//��֤����
	//float train_data[train_rows][features_cols];
	//float train_labels[train_rows];
	//float valid_data[valid_rows][features_cols];
	//float valid_labels[valid_rows];
	load_train_data((float*)train_data, (float*)train_labels);//����ѵ������
	load_valid_data((float*)valid_data, (float*)valid_labels);//������֤����


	//ת����DMatrix���ݸ�ʽ
	DMatrixHandle h_train[1];
	XGDMatrixCreateFromMat((float*)train_data, train_rows, features_cols, -1, &h_train[0]);
	//���ر�ǩ
	XGDMatrixSetFloatInfo(h_train[0], "label", train_labels, train_rows);//ѵ����Ӧ�ı�ǩ��ΪDMatrix����ǩ����

	//��ȡ�ؼ��ϵĲ���QString --> const char*
	string param_max_depth = XGBoost::ui.max_depth->text().toStdString();//��ȡתΪ�ַ���
	string param_min_child = XGBoost::ui.min_child->text().toStdString();
	string param_gamma = XGBoost::ui.gamma->text().toStdString();
	string param_subsample = XGBoost::ui.subsample->text().toStdString();
	string param_colsample = XGBoost::ui.colsample->text().toStdString();


	//����ʵ�������ò���
	BoosterHandle h_booster;
	XGBoosterCreate(h_train, 1, &h_booster);
	XGBoosterSetParam(h_booster, "booster", "gbtree");//ģ�͵���ʽ
	XGBoosterSetParam(h_booster, "objective", "reg:squarederror");//Ŀ�꺯��
	XGBoosterSetParam(h_booster, "max_depth", param_max_depth.c_str());
	XGBoosterSetParam(h_booster, "eta", "0.06");
	XGBoosterSetParam(h_booster, "min_child_weight", param_min_child.c_str());
	XGBoosterSetParam(h_booster, "subsample", param_subsample.c_str());
	XGBoosterSetParam(h_booster, "colsample_bytree", param_colsample.c_str());
	XGBoosterSetParam(h_booster, "gamma", param_gamma.c_str());
	//ִ�е�������������������������
	float MIN = FLT_MAX;//����RMSE����Сֵ
	int flag_boosters = 1;////////////////////////////////////////////////////////////////////////////////////////
	for (int iter = 1; iter <= 200; iter++)
	{
		XGBoosterUpdateOneIter(h_booster, iter, h_train[0]);
		if (iter > 0 && (iter % 4 == 0))//ʮ��������һ����֤������ģ�ͣ���ӡָ��
		{
			string fname = "./" + to_string(iter) + "xgb.model";
			XGBoosterSaveModel(h_booster, fname.c_str());
			float rmse = validate(iter, (float*)valid_data, (float*)valid_labels,ui.textBrowser);
			if (rmse < MIN)//��ǰ���������и�Сrmse
			{
				MIN = rmse;
				flag_boosters = iter;//��¼��Сrmse��Ӧ��������
			}
		}
	}
	string message = "better model:  " + to_string(flag_boosters) + "xgb.model" + "		RMSE:  " + to_string(MIN);
	//QString(QString::fromLocal8Bit(message.c_str()));//String --> QString
	ui.textBrowser->append(QString(QString::fromLocal8Bit(message.c_str())));
	QString model = QString::number(flag_boosters) + "xgb.model";
	ui.model_path->setText(model);

	//�ͷ��ڴ�
	XGDMatrixFree(h_train[0]);
	XGBoosterFree(h_booster);
	delete[]train_labels;
	delete[]train_data;
	delete[]valid_labels;
	delete[]valid_data;
	return;
}
void XGBoost::on_select_train_clicked()
{
	QString dir = QFileDialog::getOpenFileName(
		this,
		tr("ѡ���ļ�"),//����
		".",//��ǰĿ¼
		"(*.txt)");
	if (!dir.isEmpty())
	{
		ui.train_path->setText(dir);
		filepath = dir.toStdString();//���µ�ַ
	}
}
void XGBoost::on_select_valid_clicked()
{
	QString dir = QFileDialog::getOpenFileName(
		this,
		tr("ѡ���ļ�"),//����
		".",//��ǰĿ¼
		"(*.txt)");
	if (!dir.isEmpty())
	{
		ui.valid_path->setText(dir);
		valid_path = dir.toStdString();//���µ�ַ
	}
}


void load_train_data(float* train_data, float* train_labels)
{
	int cols = features_cols + 1;
	float* boston_train = new float[train_rows * cols];
	//vector<vector<float>> boston_train;
	ifstream datafile;
	datafile.open(filepath.data());//�ļ����������ļ�����
	assert(datafile.is_open());//ʧ���򱨴�
	string s;
	int i = 0, j = 0;
	while (getline(datafile, s))//���ж�ȡ
	{
		string temp;
		stringstream sstr(s);//ʹ��stringstream����
		while (getline(sstr, temp, ' '))
		{
			if (temp == "")continue;//�������Ϊ��ʿ�����ݼ��������ո񣬿��Բ�ɾ
			boston_train[i * cols + j] = atof(temp.c_str());
			j++;
			j = j % cols;//ȡ��
		}
		i++;
	}
	//��һ����Ϊ���зֲ�ʿ�����ݵ�����
	for (int i = 0; i < train_rows; i++)
	{
		for (int j = 0; j < features_cols; j++)
		{
			train_data[i * features_cols + j] = boston_train[i * cols + j];
		}
	}
	//��һ����Ϊ���зֲ�ʿ�����ݵ�����
	//����ѵ���õ���������ȫ�������train[][]�У�֮�����label������Ŀǰ��û�а�label�ϲ����ļ���
	for (int i = 0; i < train_rows; i++)
		train_labels[i] = boston_train[i * cols + features_cols];//��ʿ�ٱ�ǩ
}

void load_valid_data(float* valid_data, float* valid_labels)
{
	int cols = features_cols + 1;
	float* boston_train = new float[valid_rows * cols];
	//vector<vector<float>> boston_train;
	ifstream datafile;
	datafile.open(valid_path.data());//�ļ����������ļ�����
	assert(datafile.is_open());//ʧ���򱨴�
	string s;
	int i = 0, j = 0;
	while (getline(datafile, s))//���ж�ȡ
	{
		string temp;
		stringstream sstr(s);//ʹ��stringstream����
		while (getline(sstr, temp, ' '))
		{
			if (temp == "")continue;//�������Ϊ��ʿ�����ݼ��������ո񣬿��Բ�ɾ
			boston_train[i * cols + j] = atof(temp.c_str());
			j++;
			j = j % cols;//ȡ��
		}
		i++;
	}

	for (int i = 0; i < valid_rows; i++)
	{
		for (int j = 0; j < features_cols; j++)
		{
			valid_data[i * features_cols + j] = boston_train[i * cols + j];
		}
	}
	//��һ����Ϊ���з�����
	for (int i = 0; i < valid_rows; i++)
		valid_labels[i] = boston_train[i*cols+features_cols];//��ʿ�ٱ�ǩ
}

float validate(int iter, float *valid_data, float *valid_labels, QTextBrowser* textBrowser)
{
	string model_path = "./" + to_string(iter) + "xgb.model";
	//const int v_labels = 50, features = 13;
	//float valid_data[valid_rows][features_cols];
	//float valid_labels[valid_rows];
	load_valid_data((float *)valid_data, (float *)valid_labels);

	//���������֤���ݵĲɼ�������������Ԥ�⣬�����ǩ����ָ��
	DMatrixHandle h_valid[1];//��֤���ݣ��ޱ�ǩ

	const float *f;//����Ԥ����
	bst_ulong out_len;//�������=��ǩ����
	XGDMatrixCreateFromMat((float *)valid_data, valid_rows, features_cols, -1, &h_valid[0]);

	//����ģ��
	BoosterHandle h_booster;
	XGBoosterCreate(h_valid, 1, &h_booster);
	int ret = XGBoosterLoadModel(h_booster, model_path.c_str());
	XGBoosterPredict(h_booster, h_valid[0], 0, 0, 0, &out_len, &f);  //����Ԥ��
	
	float rmse = RMSE(f, valid_labels, out_len);

	string message = "iter:" + to_string(iter) + "    RMSE:" + to_string(rmse);
	//QString(QString::fromLocal8Bit(message.c_str()));//String --> QString
	textBrowser->append(QString(QString::fromLocal8Bit(message.c_str())));
	//cout << "iter:" << iter << "    RMSE:" << rmse << endl;//��ӡRMSEָ��
	XGDMatrixFree(h_valid[0]);
	XGBoosterFree(h_booster);
	return rmse;
}

float RMSE(const float * Data, float *Data2, int Num)
{
	double fSum = 0;
	for (int i = 0; i < Num; ++i)
	{
		fSum += (Data[i] - Data2[i]) *(Data[i] - Data2[i]);
	}
	return sqrt(fSum / Num);
}
