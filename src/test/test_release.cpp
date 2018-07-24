//
// Created by jiang.wenqiang on 2018/7/23.
//

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    qDebug("debug");
    return QCoreApplication::exec();
}