//
// Created by jiang.wenqiang on 2018/8/20.
//

#include <QtCore/QDebug>
#include "Curve.h"
#include "Can.h"
#include "Transforo.h"
#include "Transmit.h"
#include "Revolvo.h"
#include "Load.h"

int main() {
    Can::Config config(0);
    Can can(&config);
    Buffer buffer;
    Curve curve;
    QFile f(QString("D:/jiang.wenqiang/code/refine/config/配置.csv"));
    curve.load(f, Curve::FileType::Csv);
    Tribe tribe;
    Transmit transmit(&can, &buffer);
    Transforo transforo(&curve, &buffer, &tribe);
    Revolvo revolvo(&transmit, &transforo);
    Load load;
    QFile data(QString("D:/jiang.wenqiang/code/refine/config/数据.csv"));
    load.setFile(&data, Load::FileType::Csv);
    load.setTribe(&tribe);
    load.start();
    while (load.isRunning()) {}
}