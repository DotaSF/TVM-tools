#include "XGBoost.h"
#include"ui_XGBoost.h"
#include <QtWidgets/QApplication>
#include <Windows.h>
#include <QtGui>
#include<QMessageBox>
#include<iostream>
#include<xgboost/c_api.h>
//多线程
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

string filepath;//获取训练文件地址
string valid_path;//获取验证文件地址
int train_rows;
int valid_rows;
int features_cols;
void XGBoost::on_optimize_clicked()//自动优化
{
	//首先需要读取参数
	QString train_path_ = ui.train_path->text(); filepath = train_path_.toStdString();//获取训练文件地址
	QString valid_path_ = ui.valid_path->text(); valid_path = valid_path_.toStdString();//获取验证文件地址
	QString train_rows_ = ui.train_rows->text(); train_rows = train_rows_.toInt();
	QString valid_rows_ = ui.valid_rows->text(); valid_rows = valid_rows_.toInt();
	QString features_cols_ = ui.features_cols->text(); features_cols = features_cols_.toInt();

	const int num_booster = 200;//树的数量
	float* train_labels = new float[train_rows];//训练标签数组
	float* train_data = new float[train_rows * features_cols];//训练数据
	float* valid_labels = new float[valid_rows];//验证标签数组
	float* valid_data = new float[valid_rows * features_cols];//验证数据
	load_train_data((float*)train_data, (float*)train_labels);//加载训练数据
	load_valid_data((float*)valid_data, (float*)valid_labels);//加载验证数据

	//转换到DMatrix数据格式
	DMatrixHandle h_train[1];
	XGDMatrixCreateFromMat((float*)train_data, train_rows, features_cols, -1, &h_train[0]);
	//加载标签
	XGDMatrixSetFloatInfo(h_train[0], "label", train_labels, train_rows);//训练对应的标签变为DMatrix，标签个数
	//创建实例并设置参数
	BoosterHandle h_booster;
	XGBoosterCreate(h_train, 1, &h_booster);
	XGBoosterSetParam(h_booster, "booster", "gbtree");//模型的形式
	XGBoosterSetParam(h_booster, "objective", "reg:squarederror");//目标函数
	XGBoosterSetParam(h_booster, "max_depth", "5");
	XGBoosterSetParam(h_booster, "eta", "0.1");
	XGBoosterSetParam(h_booster, "min_child_weight", "1");
	XGBoosterSetParam(h_booster, "subsample", "0.8");
	XGBoosterSetParam(h_booster, "colsample_bytree", "0.8");
	XGBoosterSetParam(h_booster, "gamma", "0");
	//执行迭代，迭代次数就是树的数量
	float MIN = FLT_MAX;//储存RMSE的最小值
	int flag_boosters = 1;////////////////////////////////////////////////////////////////////////////////////////
	for (int iter = 1; iter <= num_booster; iter++)
	{
		XGBoosterUpdateOneIter(h_booster, iter, h_train[0]);
		if (iter > 0 && (iter % 4 == 0))//4棵树验证一次
		{
			string fname = "./" + to_string(iter) + "xgb.model";
			XGBoosterSaveModel(h_booster, fname.c_str());
			float rmse = validate(iter, (float*)valid_data, (float*)valid_labels, ui.textBrowser);
			if (rmse < MIN)//当前迭代次数有更小rmse
			{
				MIN = rmse;
				flag_boosters = iter;//记录最小rmse对应的树数量
			}
		}
	}
	XGBoosterFree(h_booster);//释放句柄

	string message = "iterations:  " + to_string(flag_boosters) + "		RMSE:  " + to_string(MIN);
	ui.textBrowser->append(QString(QString::fromLocal8Bit(message.c_str())));
	//至此寻找完boosters的数量，进行max_depth 和 min_child_weight调优，分别代表树的最大深度和叶子结点最少样本数
	//搜索范围：max_depth->[3,10]，min_child_weight->[1,6]
	int depth1 = 3, depth2 = 10, weight1 = 1, weight2 = 6;
	int max_depth = 3, min_child_weight = 1;//////////////////////////////////////////////////////////////////////////////
	MIN = FLT_MAX;//储存RMSE的最小值
	int zuo1 = depth1;
	for (; zuo1 <= depth2; zuo1++)//栅格搜索
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
			XGBoosterSetParam(h_booster, "booster", "gbtree");//模型的形式
			XGBoosterSetParam(h_booster, "objective", "reg:squarederror");//目标函数
			XGBoosterSetParam(h_booster, "eta", "0.1");
			XGBoosterSetParam(h_booster, "subsample", "0.8");
			XGBoosterSetParam(h_booster, "colsample_bytree", "0.8");
			XGBoosterSetParam(h_booster, "gamma", "0");

			//执行迭代，迭代次数是之前调试出来的
			for (int iter = 1; iter <= flag_boosters; iter++)
			{
				XGBoosterUpdateOneIter(h_booster, iter, h_train[0]);
			}
			cout << canshu << "    " << canshu2 << "    ";
			string fname = "./" + to_string(flag_boosters) + "xgb.model";
			XGBoosterSaveModel(h_booster, fname.c_str());

			float rmse = validate(flag_boosters, (float*)valid_data, (float*)valid_labels, ui.textBrowser);
			if (rmse < MIN)//当前迭代次数有更小rmse
			{
				MIN = rmse;
				max_depth = zuo1;
				min_child_weight = zuo2;
			}
			//释放数据结构
			XGBoosterFree(h_booster);
		}
	}
	string message1 = "max_depth: " + to_string(max_depth) + "  min_child_weight:" + to_string(min_child_weight);
	ui.textBrowser->append(QString(QString::fromLocal8Bit(message1.c_str())));

	//至此完成max_depth 和 min_child_weight的调优，进行gamma的调优，正则化参数
	//搜索范围：gamma->[0.0,1.0]，这个范围可以是[0,正无穷]，这个参数的值越大，算法越保守
	float gamma1 = 0.0, gamma2 = 1.0;//左右值
	float gamma = 0.0;//////////////////////////////////////////////////////////////////////////////////////////////////
	MIN = FLT_MAX;//储存RMSE的最小值
	float left1 = gamma1;
	for (; left1 <= gamma2; left1 += 0.1)//栅格搜索
	{
		string canshu = to_string(left1);
		BoosterHandle h_booster;
		XGBoosterCreate(h_train, 1, &h_booster);
		XGBoosterSetParam(h_booster, "booster", "gbtree");//模型的形式
		XGBoosterSetParam(h_booster, "objective", "reg:squarederror");//目标函数
		XGBoosterSetParam(h_booster, "max_depth", to_string(max_depth).c_str());
		XGBoosterSetParam(h_booster, "eta", "0.1");
		XGBoosterSetParam(h_booster, "min_child_weight", to_string(min_child_weight).c_str());
		XGBoosterSetParam(h_booster, "subsample", "0.8");
		XGBoosterSetParam(h_booster, "colsample_bytree", "0.8");
		XGBoosterSetParam(h_booster, "gamma", canshu.c_str());

		//执行迭代，迭代次数是之前调试出来的
		for (int iter = 1; iter <= flag_boosters; iter++)
		{
			XGBoosterUpdateOneIter(h_booster, iter, h_train[0]);
		}
		cout << canshu << "    ";
		string fname = "./" + to_string(flag_boosters) + "xgb.model";
		XGBoosterSaveModel(h_booster, fname.c_str());
		float rmse = validate(flag_boosters, (float*)valid_data, (float*)valid_labels, ui.textBrowser);
		if (rmse < MIN)//当前迭代次数有更小rmse
		{
			MIN = rmse;
			gamma = left1;
		}
		//释放数据结构
		XGBoosterFree(h_booster);
	}
	string message2 = "gamma:  " + to_string(gamma);
	ui.textBrowser->append(QString(QString::fromLocal8Bit(message2.c_str())));


	//至此完成gamma调参，接下来是subsample 和 colsample_bytree调优。subsample控制对于每棵树，训练样本随机采样的比例；colsample_bytree控制每棵随机采样的特征的占比
	float subsample1 = 0.5, subsample2 = 1.0, colsample_bytree1 = 0.5, colsample_bytree2 = 1.0;
	float subsample = 0.5, colsample_bytree = 0.5;//////////////////////////////////////////////////////////////////////////////
	MIN = FLT_MAX;//储存RMSE的最小值
	left1 = subsample1;
	for (; left1 <= subsample2; left1 += 0.05)//栅格搜索
	{
		float left2 = colsample_bytree1;
		for (; left2 <= colsample_bytree2; left2 += 0.05)
		{
			string canshu = to_string(left1);
			string canshu2 = to_string(left2);
			BoosterHandle h_booster;
			XGBoosterCreate(h_train, 1, &h_booster);
			XGBoosterSetParam(h_booster, "booster", "gbtree");//模型的形式
			XGBoosterSetParam(h_booster, "objective", "reg:squarederror");//目标函数
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
			if (rmse < MIN)//当前迭代次数有更小rmse
			{
				MIN = rmse;
				subsample = left1;
				colsample_bytree = left2;
			}
			//释放数据结构
			XGBoosterFree(h_booster);
		}
	}
	string message3 = "subsample:  " + to_string(subsample) + "	colsample_bytree:  " + to_string(colsample_bytree);
	ui.textBrowser->append(QString(QString::fromLocal8Bit(message1.c_str())));
	ui.textBrowser->append(QString(QString::fromLocal8Bit(message2.c_str())));
	ui.textBrowser->append(QString(QString::fromLocal8Bit(message3.c_str())));

	//释放内存
	delete[]train_labels;
	delete[]train_data;
	delete[]valid_labels;
	delete[]valid_data;
	XGDMatrixFree(h_train[0]);
	return;
}
void XGBoost::on_train_clicked()//训练
{
	QString train_path_ = ui.train_path->text(); filepath = train_path_.toStdString();//获取训练文件地址
	QString valid_path_ = ui.valid_path->text(); valid_path = valid_path_.toStdString();//获取验证文件地址
	QString train_rows_ = ui.train_rows->text(); train_rows = train_rows_.toInt();
	QString valid_rows_ = ui.valid_rows->text(); valid_rows = valid_rows_.toInt();
	QString features_cols_ = ui.features_cols->text(); features_cols = features_cols_.toInt();
	const int num_booster = 200;//树的数量

	float* train_labels = new float[train_rows];//训练标签数组
	float* train_data = new float[train_rows* features_cols];//训练数据
	float* valid_labels = new float[valid_rows];//验证标签数组
	float* valid_data = new float[valid_rows * features_cols];//验证数据
	//float train_data[train_rows][features_cols];
	//float train_labels[train_rows];
	//float valid_data[valid_rows][features_cols];
	//float valid_labels[valid_rows];
	load_train_data((float*)train_data, (float*)train_labels);//加载训练数据
	load_valid_data((float*)valid_data, (float*)valid_labels);//加载验证数据


	//转换到DMatrix数据格式
	DMatrixHandle h_train[1];
	XGDMatrixCreateFromMat((float*)train_data, train_rows, features_cols, -1, &h_train[0]);
	//加载标签
	XGDMatrixSetFloatInfo(h_train[0], "label", train_labels, train_rows);//训练对应的标签变为DMatrix，标签个数

	//获取控件上的参数QString --> const char*
	string param_max_depth = XGBoost::ui.max_depth->text().toStdString();//读取转为字符串
	string param_min_child = XGBoost::ui.min_child->text().toStdString();
	string param_gamma = XGBoost::ui.gamma->text().toStdString();
	string param_subsample = XGBoost::ui.subsample->text().toStdString();
	string param_colsample = XGBoost::ui.colsample->text().toStdString();


	//创建实例并设置参数
	BoosterHandle h_booster;
	XGBoosterCreate(h_train, 1, &h_booster);
	XGBoosterSetParam(h_booster, "booster", "gbtree");//模型的形式
	XGBoosterSetParam(h_booster, "objective", "reg:squarederror");//目标函数
	XGBoosterSetParam(h_booster, "max_depth", param_max_depth.c_str());
	XGBoosterSetParam(h_booster, "eta", "0.06");
	XGBoosterSetParam(h_booster, "min_child_weight", param_min_child.c_str());
	XGBoosterSetParam(h_booster, "subsample", param_subsample.c_str());
	XGBoosterSetParam(h_booster, "colsample_bytree", param_colsample.c_str());
	XGBoosterSetParam(h_booster, "gamma", param_gamma.c_str());
	//执行迭代，迭代次数就是树的数量
	float MIN = FLT_MAX;//储存RMSE的最小值
	int flag_boosters = 1;////////////////////////////////////////////////////////////////////////////////////////
	for (int iter = 1; iter <= 200; iter++)
	{
		XGBoosterUpdateOneIter(h_booster, iter, h_train[0]);
		if (iter > 0 && (iter % 4 == 0))//十棵树进行一次验证，保存模型，打印指标
		{
			string fname = "./" + to_string(iter) + "xgb.model";
			XGBoosterSaveModel(h_booster, fname.c_str());
			float rmse = validate(iter, (float*)valid_data, (float*)valid_labels,ui.textBrowser);
			if (rmse < MIN)//当前迭代次数有更小rmse
			{
				MIN = rmse;
				flag_boosters = iter;//记录最小rmse对应的树数量
			}
		}
	}
	string message = "better model:  " + to_string(flag_boosters) + "xgb.model" + "		RMSE:  " + to_string(MIN);
	//QString(QString::fromLocal8Bit(message.c_str()));//String --> QString
	ui.textBrowser->append(QString(QString::fromLocal8Bit(message.c_str())));
	QString model = QString::number(flag_boosters) + "xgb.model";
	ui.model_path->setText(model);

	//释放内存
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
		tr("选择文件"),//标题
		".",//当前目录
		"(*.txt)");
	if (!dir.isEmpty())
	{
		ui.train_path->setText(dir);
		filepath = dir.toStdString();//更新地址
	}
}
void XGBoost::on_select_valid_clicked()
{
	QString dir = QFileDialog::getOpenFileName(
		this,
		tr("选择文件"),//标题
		".",//当前目录
		"(*.txt)");
	if (!dir.isEmpty())
	{
		ui.valid_path->setText(dir);
		valid_path = dir.toStdString();//更新地址
	}
}


void load_train_data(float* train_data, float* train_labels)
{
	int cols = features_cols + 1;
	float* boston_train = new float[train_rows * cols];
	//vector<vector<float>> boston_train;
	ifstream datafile;
	datafile.open(filepath.data());//文件流对象与文件链接
	assert(datafile.is_open());//失败则报错
	string s;
	int i = 0, j = 0;
	while (getline(datafile, s))//逐行读取
	{
		string temp;
		stringstream sstr(s);//使用stringstream方法
		while (getline(sstr, temp, ' '))
		{
			if (temp == "")continue;//这句是因为波士顿数据集有两个空格，可以不删
			boston_train[i * cols + j] = atof(temp.c_str());
			j++;
			j = j % cols;//取余
		}
		i++;
	}
	//这一段是为了切分波士顿数据的特征
	for (int i = 0; i < train_rows; i++)
	{
		for (int j = 0; j < features_cols; j++)
		{
			train_data[i * features_cols + j] = boston_train[i * cols + j];
		}
	}
	//这一段是为了切分波士顿数据的特征
	//至此训练用的特征数据全部保存进train[][]中，之后加上label，大橘目前还没有把label合并进文件中
	for (int i = 0; i < train_rows; i++)
		train_labels[i] = boston_train[i * cols + features_cols];//波士顿标签
}

void load_valid_data(float* valid_data, float* valid_labels)
{
	int cols = features_cols + 1;
	float* boston_train = new float[valid_rows * cols];
	//vector<vector<float>> boston_train;
	ifstream datafile;
	datafile.open(valid_path.data());//文件流对象与文件链接
	assert(datafile.is_open());//失败则报错
	string s;
	int i = 0, j = 0;
	while (getline(datafile, s))//逐行读取
	{
		string temp;
		stringstream sstr(s);//使用stringstream方法
		while (getline(sstr, temp, ' '))
		{
			if (temp == "")continue;//这句是因为波士顿数据集有两个空格，可以不删
			boston_train[i * cols + j] = atof(temp.c_str());
			j++;
			j = j % cols;//取余
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
	//这一段是为了切分特征
	for (int i = 0; i < valid_rows; i++)
		valid_labels[i] = boston_train[i*cols+features_cols];//波士顿标签
}

float validate(int iter, float *valid_data, float *valid_labels, QTextBrowser* textBrowser)
{
	string model_path = "./" + to_string(iter) + "xgb.model";
	//const int v_labels = 50, features = 13;
	//float valid_data[valid_rows][features_cols];
	//float valid_labels[valid_rows];
	load_valid_data((float *)valid_data, (float *)valid_labels);

	//至此完成验证数据的采集，接下来进行预测，并与标签计算指标
	DMatrixHandle h_valid[1];//验证数据，无标签

	const float *f;//储存预测结果
	bst_ulong out_len;//结果长度=标签个数
	XGDMatrixCreateFromMat((float *)valid_data, valid_rows, features_cols, -1, &h_valid[0]);

	//加载模型
	BoosterHandle h_booster;
	XGBoosterCreate(h_valid, 1, &h_booster);
	int ret = XGBoosterLoadModel(h_booster, model_path.c_str());
	XGBoosterPredict(h_booster, h_valid[0], 0, 0, 0, &out_len, &f);  //批量预测
	
	float rmse = RMSE(f, valid_labels, out_len);

	string message = "iter:" + to_string(iter) + "    RMSE:" + to_string(rmse);
	//QString(QString::fromLocal8Bit(message.c_str()));//String --> QString
	textBrowser->append(QString(QString::fromLocal8Bit(message.c_str())));
	//cout << "iter:" << iter << "    RMSE:" << rmse << endl;//打印RMSE指标
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
