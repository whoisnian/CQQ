#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    #if defined(Q_OS_MACOS)
        QApplication::setFont(QFont("PingFang"));
        QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
        QApplication::setQuitOnLastWindowClosed(false);
    #endif
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
