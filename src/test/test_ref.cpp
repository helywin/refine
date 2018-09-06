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
    Curve cc;
    QFile curve_file(QString("D:/jiang.wenqiang/code/refine/config/配置.csv"));
    c.load(curve_file, Curve::FileType::Csv);
    QFile f(QString("D:/jiang.wenqiang/code/refine/data/a.vc"));
    ref.dumpCurveConfig(f, c);
    ref.loadCurveConfig(f, cc);
    qDebug() << cc.str();
    return 0;
}