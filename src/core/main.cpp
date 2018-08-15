//
// Created by jiang.wenqiang on 2018/8/3.
//

#include <QtCore/QDebug>
#include <QtTest/QTest>
#include "Curve.h"
#include "Can.h"
#include "Buffer.h"
#include "Collect.h"
#include "Revolvi.h"
#include "Transfori.h"

int main() {
    Curve curve;
    curve.appendRow();
    curve.appendRow(Curve::Bundle::Acceleration);
//    QString s = QString("加速度");
//    curve[0].setNameByStr(s);
    qDebug() << curve.header().join(QChar(','));
    qDebug() << curve[1].str();
    return 0;
}