//
// Created by jiang.wenqiang on 2018/7/3.
//
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include "Revolve.h"
#include "Transform.h"


int main() {
//    qInstallMessageHandler(Log::handler);
//    Log::setPath(QString("D:/jiang.wenqiang/code/refine/log/log.txt"));
    qInfo("===============启动测试采集==============");
    Can::Config config;
    Can can(config);
//    ErrorInfo error;
    Buffer buffer(20, 200);
    QFile f(":/res/test/cfg.txt");
    Curve curve;
    curve.loadCsv(f);
    QStringList list;
    curve.str(list);
    for (const auto &iter : list) {
        qDebug() << iter;
    }
    Kebab data((unsigned short)curve.size());
    QFile file(QString("D:/jiang.wenqiang/code/refine/data/data.txt"));
    Keep keep(data, file);
    Collect collect(can, buffer);
    Transform transform(curve);
    Sublime sublime(buffer, data, transform);
    Revolve revolve(keep, collect, sublime);
    revolve.marvel();
    return 0;
}