//
// Created by jiang.wenqiang on 2018/7/30.
//

#include <QtWidgets/QApplication>
#include "SendDialog.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QApplication::setStyle("fusion");
    SendDialog dialog;
    dialog.show();
    return QApplication::exec();
}