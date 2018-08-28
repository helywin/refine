//
// Created by jiang.wenqiang on 2018/8/22.
//

#include <QtCore/QDebug>
#include <cstdio>
#include "Curve.h"
#include "Can.h"
#include "Transfori.h"
#include "Revolvi.h"
#include "Dump.h"
#include "Collect.h"

int main() {
    Can::Config config(1);
    Can can(&config);
    can.close();
//    if(can.open()){
//        qDebug("打开化CAN盒成功");
//    }
//    getchar();
//    if(can.init()){
//        qDebug("初始化通道2成功");
//    }
    can.connect();
//    can.reset();
//    if(can.start()){
//        qDebug("启动通道2成功");
//    }
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

    buffer.headCell()->setDataSize(5);
    buffer.headCell()->setSendType(
            Buffer::Cell::SendType::SelfSendRecieve);
    buffer.headForward();

    buffer.headCell()->setDataSize(5);
    buffer.headCell()->setSendType(
            Buffer::Cell::SendType::SelfSendRecieve);
    buffer.headForward();

    revolvi.marvel();

    return 0;
}