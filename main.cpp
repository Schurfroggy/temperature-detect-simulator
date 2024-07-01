#include "mainwindow.h"
#include "windows.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    srand(static_cast<unsigned int>(time(nullptr)));
    QApplication a(argc, argv);
    QTimer* timer =new QTimer();
    MainWindow w(timer);

    w.tickFlush();
    w.show();
    timer->start();

    return a.exec();
}
