//
// Created by jiang.wenqiang on 2018/7/3.
//
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include "Revolve.h"


int main() {
//    qInstallMessageHandler(Log::handler);
//    Log::set_path(QString("D:/jiang.wenqiang/code/refine/log/log.txt"));
    qInfo("===============启动测试采集==============");
    CanConfig config;
    Can can(config);
//    CanErrorInfo error;
    CanBuffer buffer(20, 200);
    QFile f(":/res/test/cfg.txt");
    CurveConfig curve;
    curve.load(f);
    QStringList list;
    curve.str(list);
    for (const auto &iter : list) {
        qDebug() << iter;
    }
    Kebab data((unsigned short)curve.size());
    QFile file(QString("D:/jiang.wenqiang/code/refine/data/data.txt"));
    Keep keep(data, file);
    Collect collect(can, buffer);
    Sublime sublime(curve, buffer, data);
    Revolve revolve(keep, collect, sublime);
    revolve.marvel();
    return 0;
}