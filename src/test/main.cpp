//
// Created by jiang.wenqiang on 2018/9/10.
//

#include <QtWidgets/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QFontDatabase>
#include "Refine.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QFontDatabase::addApplicationFont("fonts/ubuntu-mono.ttf");
    QTranslator t;
    t.load(":/res/trans/qt_zh_CN.qm");
    QApplication::installTranslator(&t);
    Refine refine;
    refine.show();
    return QApplication::exec();
}