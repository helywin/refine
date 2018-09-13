//
// Created by jiang.wenqiang on 2018/9/13.
//

#include <QtCore/QDebug>
#include "File.h"
#include "Curve.h"

int main() {
    Curve curve;
    Curve curve1;
    curve.loadFromCsv(QString("D:/jiang.wenqiang/code/refine/config/配置.csv"));
    QFile cvc("D:/jiang.wenqiang/code/refine/data/a.cvc");
    File f;
    f.dumpCurveConfig(cvc, curve);
    f.loadCurveConfig(cvc, curve1);
    qDebug() << curve.str();
    qDebug() << curve1.str();
    return 0;
}
