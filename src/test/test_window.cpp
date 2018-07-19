//
// Created by jiang.wenqiang on 2018/7/19.
//

#include "Window.h"
#include <QtCore/QDebug>
#include "QtWidgets/QApplication"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Window win;
    win.show();
    QApplication::setStyle("fusion");
    return QApplication::exec();
}