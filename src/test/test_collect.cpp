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

int main()
{
    Can::Config cfg(0);
    Can can(&cfg);
    Curve curve;
    curve.loadFromCsv(QString("D:/jiang.wenqiang/code/refine/config/配置.csv"));
    QFile file_frames(QString("D:/jiang.wenqiang/code/refine/data/data.fmd"));
    Buffer buffer;
    Collect collect(&can, &buffer);
    Tribe tribe;
    Transform transform(&buffer, &curve, &tribe, &file_frames, true);
    transform.initializeFramesStored();
    Revolve revolve(&collect, &transform, nullptr);
    qDebug() << can.connect();
    revolve.run();
    transform.finishFramesStored();
    return 0;
}