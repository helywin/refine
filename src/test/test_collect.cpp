//
// Created by jiang.wenqiang on 2018/7/3.
//
#include <QtCore/QFile>
#include "Revolve.h"
#include "Kebab.h"
#include "Log.h"
#include "CanConfig.h"
#include "Can.h"
#include "CurveConfig.h"
#include <iostream>


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
    Sublime sublime(curve, buffer, data);
    Revolve revolve;
    revolve.set(&keep, &collect, &sublime);
    revolve.marvel();
    return 0;
}