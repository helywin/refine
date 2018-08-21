//
// Created by jiang.wenqiang on 2018/8/20.
//

#include <QtCore/QDebug>
#include <iostream>
#include "Curve.h"
#include "Can.h"
#include "Transforo.h"
#include "Transmit.h"
#include "Revolvo.h"
#include "Load.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        qDebug() << "请输入两个参数 [配置文件名] [数据文件名]";
    }
    QFile f_conf;
    QFile f_data;
    if (argc == 1) {
        f_conf.setFileName("D:/jiang.wenqiang/code/refine/config/配置.csv");
        f_data.setFileName("D:/jiang.wenqiang/code/refine/config/数据.csv");
    } else  if (argc == 3){
        f_conf.setFileName(argv[1]);
        f_data.setFileName(argv[2]);
    } else {
        return -1;
    }
    Can::Config config(0);
    Can can(&config);
    can.connect();
    Buffer buffer;
    Curve curve;
    curve.load(f_conf, Curve::FileType::Csv);
    Tribe tribe;
    Transmit transmit(&can, &buffer);
    Transforo transforo(&curve, &buffer, &tribe);
    Revolvo revolvo(&transmit, &transforo);
    Load load;
    load.setFile(&f_data, Load::FileType::Csv);
    load.setTribe(&tribe);
    qDebug() << "正在加载数据..";
    load.start();
    while (load.isRunning()) {}
    qDebug() << "加载数据完毕";
    qDebug() << "正在发送数据..";
    revolvo.marvel();
    qDebug() << "发送数据完毕";
    return 0;
}