//
// Created by jiang.wenqiang on 2018/7/3.
//
#include <QtCore/QFile>
<<<<<<< HEAD
#include "../src/Revolve.h"
#include "Kebab.h"
#include "../src/Log.h"
#include "../src/CanConfig.h"
#include "../src/Can.h"
#include "../src/CurveConfig.h"
#include <iostream>
=======
#include <QtCore/QDebug>
#include "Revolve.h"
#include "Transform.h"
>>>>>>> 568e96c... 准备重新开发结构


int main() {
//    qInstallMessageHandler(Log::handler);
//    Log::set_path(QString("D:/jiang.wenqiang/code/autophi/core/log.txt"));
    qInfo("===============启动测试采集==============");
    CanConfig config;
    Can can(config);
    CanErrorInfo error;
    CanBuffer buffer(50, 100);
    QFile f("cfg.txt");
    CurveConfig curve;
    curve.load(f);
    QStringList list;
    curve.str(list);
    for (const auto &iter : list) {
        qDebug() << iter;
    }
    Kebab data(20);
    QFile file(QString("D:/jiang.wenqiang/code/autophi/core/data.txt"));
    Keep keep(data, file);
    Collect collect(can, buffer);
<<<<<<< HEAD
    Sublime sublime(curve, buffer, data);
    Revolve revolve;
    revolve.set(&keep, &collect, &sublime);
=======
    Transform transform(curve);
    Sublime sublime(buffer, data, transform);
    Revolve revolve(keep, collect, sublime);
>>>>>>> 568e96c... 准备重新开发结构
    revolve.marvel();
    return 0;
}