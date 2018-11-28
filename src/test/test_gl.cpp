//
// Created by jiang.wenqiang on 2018/11/23.
//

#include <QtWidgets/QApplication>
#include "MyOpenGLWidget.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MyOpenGLWidget widget;
    widget.show();
    return QApplication::exec();
}