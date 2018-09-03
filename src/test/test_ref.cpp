//
// Created by jiang.wenqiang on 2018/8/31.
//

#include "Ref.h"
#include "Curve.h"
#include <QtCore/QDebug>
#include <QtCore/QDateTime>

int main() {
    Ref ref;
    Curve c;
    QFile f(QString("D:/jiang.wenqiang/code/refine/data/a.vc"));
    ref.dumpCurveConfig(f, c);
    ref.loadCurveConfig(f, c);
    return 0;
}