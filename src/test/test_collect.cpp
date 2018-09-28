//
// Created by jiang.wenqiang on 2018/9/20.
//

#include <QtCore/QDebug>
#include "Buffer.hpp"
#include "Curve.hpp"
#include "Can.hpp"
#include "Revolve.hpp"
#include "Tribe.hpp"
#include "Collect.hpp"

#define FLAG 0

int main()
{
#if FLAG == 1
    Curve curve;
    curve.loadFromCsv(QString("D:/jiang.wenqiang/code/refine/config/配置.csv"));
    QFile file_frames_collect(QString("D:/jiang.wenqiang/code/refine/data/data.fmd"));
    QFile file_frames(QString("D:/jiang.wenqiang/code/refine/data/data1.fmd"));
    Buffer buffer;
    Collect collect(nullptr, &buffer);
    collect.setMode(Collect::FromFile, 0, &file_frames_collect);
    Tribe tribe;
    Transform transform(&buffer, &curve, &tribe, &file_frames, true);
    transform.initializeFramesStored();
    Revolve revolve(&collect, &transform, nullptr);
    revolve.run();
    transform.finishFramesStored();
#else
    Can::Config cfg(0);
    Can can(&cfg);
    Curve curve;
    curve.loadFromCsv(QString("D:/jiang.wenqiang/code/refine/config/配置.csv"));
    QFile file_frames(QString("D:/jiang.wenqiang/code/refine/data/data.fmd"));
    Buffer buffer;
    Collect collect(&can, &buffer);
    Tribe tribe;
    for (const auto &iter : curve) {
        tribe.append(iter.name());
    }
    Transform transform(&buffer, &curve, &tribe, &file_frames, true);
    Revolve revolve(&collect, &transform, nullptr);
    if (!can.connect()) {
        qDebug("can没连接！");
        return -1;
    }
    revolve.startRevolve();
    while (revolve.isRunning()) {}
    can.close();
    tribe.dumpToCsv("D:/jiang.wenqiang/code/refine/data/data.csv");
#endif
    return 0;
}