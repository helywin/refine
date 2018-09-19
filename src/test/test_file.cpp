//
// Created by jiang.wenqiang on 2018/9/13.
//

#include <QtCore/QDebug>
#include <QtCore/QDataStream>
#include "File.hpp"
#include "Curve.hpp"
#include "Can.hpp"
#include "Buffer.hpp"

int main() {
    Curve curve;
    Curve curve1;
    curve.loadFromCsv(QString("D:/jiang.wenqiang/code/refine/config/配置.csv"));
    QFile cvc("D:/jiang.wenqiang/code/refine/data/a.cvc");
    File f;
    f.dumpCurveConfig(cvc, curve);
    f.loadCurveConfig(cvc, curve1);
    qDebug() << curve.str();
    for (auto &it : curve) {
        qDebug() << it.str();
    }
    QFile objf("D:/jiang.wenqiang/code/refine/data/obj.cvc");
    objf.open(QIODevice::WriteOnly);
    QDataStream s(&objf);
    Buffer buffer;
    for (auto &iter : buffer) {

    }
    return 0;
}
