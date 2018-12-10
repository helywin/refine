//
// Created by jiang.wenqiang on 2018/9/10.
//

#include <QtWidgets/QApplication>
#include <QtCore/QTranslator>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include "Refine.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    /*
    QFile qss(":res/qss/flatwhite.css");
    QString style;
    QTextStream stream(&qss);
    qss.open(QIODevice::ReadOnly | QIODevice::Text);
    if (qss.isOpen()) {
        style = stream.readAll();
        app.setStyleSheet(style);
    }
    */
    QTranslator t;
    t.load(":/res/trans/qt_zh_CN.qm");
    QApplication::installTranslator(&t);
    Refine refine;
    refine.show();
    return QApplication::exec();
}