#include "widget.h"
#include <QApplication>

float rec_arr[500000];
int rec_arr_cnt;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
