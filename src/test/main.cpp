//
// Created by jiang.wenqiang on 2018/9/10.
//

#include <QtWidgets/QApplication>
#include <QtCore/QTranslator>
#include "Refine.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Refine refine;
    refine.show();
    return QApplication::exec();
}