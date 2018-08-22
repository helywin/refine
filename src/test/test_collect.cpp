//
// Created by jiang.wenqiang on 2018/8/22.
//

#include <QtCore/QDebug>
#include "Curve.h"
#include "Can.h"
#include "Transfori.h"
#include "Revolvi.h"
#include "Dump.h"
#include "Collect.h"

int main() {
    Can::Config config(0);
    Can can(&config);
    can.connect();
    Buffer buffer;
    Curve curve;
    QFile f_config(QString("D:/jiang.wenqiang/code/refine/config/配置.csv"));
    QFile f_data(QString("D:/jiang.wenqiang/code/refine/config/采集数据.csv"));
    curve.load(f_config, Curve::FileType::Csv);
    Dump dump;
    dump.setFile(&f_data, Dump::FileType::Csv);
    Kebab kebab(curve.header(), &dump);
    Transfori transfori(&curve, &buffer, &kebab);
    Collect collect(&can, &buffer);
    Revolvi revolvi(&collect, &transfori);
    revolvi.marvel();
    return 0;
}