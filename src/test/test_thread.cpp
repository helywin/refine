//
// Created by jiang.wenqiang on 2018/10/15.
//

#include "TestThread.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QTimer timer;
    Print print;
    QObject::connect(&timer, &QTimer::timeout, &print, &Print::startPrint);

    timer.setInterval(1000);
    timer.start();
    while (print.isRunning()) {}
    return QCoreApplication::exec();
}