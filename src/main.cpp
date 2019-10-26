#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    int ret = 0;
    do
    {
        MainWindow w;
        w.show();
        w.init();
        ret = a.exec();
    }
    while(ret == 233);

    return ret;
}
