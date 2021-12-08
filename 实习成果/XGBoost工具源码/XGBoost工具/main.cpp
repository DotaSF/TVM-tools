#include "XGBoost.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XGBoost w;
    w.show();
    return a.exec();
}
