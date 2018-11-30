//
// Created by jiang.wenqiang on 2018/11/23.
//

#include <QtCore/QDebug>
#include <QtWidgets/QApplication>
#include "MyOpenGLWidget.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MyOpenGLWidget widget;
    int s[2][5];
    qDebug() << "sizeof(s): " << sizeof(s);
    widget.show();
    return QApplication::exec();
}