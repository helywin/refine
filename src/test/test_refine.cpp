//
// Created by jiang.wenqiang on 2018/7/12.
//

#include "Tribe.h"
#include "Refine.h"
#include "Log.h"
#include "Abstract.h"
#include "Constant.h"

int main() {
//    qInstallMessageHandler(Log::handler);
//    Log::set_path(QString("D:/jiang.wenqiang/code/autophi/core/log.txt"));
    qInfo("===============启动预处理==============");
    qDebug() << "测试常数:" << Const::Ratio::one;
    QFile f("D:/jiang.wenqiang/code/autophi/core/data/datas.csv");
    QFile fs("D:/jiang.wenqiang/code/autophi/core/data/datass.csv");
    Tribe tribe;
    Abstract abstract(tribe);
    abstract.read_csv(f);

    tribe.add("filter");
    Refine::filter(tribe["VehSpeed(NULL)"], tribe["filter"], 100, 0, 5, true);
//    qInfo("===============加速度数据==============");
//    qDebug() << tribe.str(QString("加速度(NULL)"));
//    qInfo("===============滤波后数据==============");
//    qDebug() << tribe.str(QString("filter"));
//    qInfo("===============其他数据==============");
//    Range a(0, (unsigned)tribe["加速度(NULL)"].size());
//    double avg = Refine::average(tribe["加速度(NULL)"], a);
//    qDebug() << avg;
//    double rms = Refine::rms(tribe["加速度(NULL)"], a);
//    qDebug() << rms;
//    double vdv = Refine::vdv(tribe["加速度(NULL)"], a);
//    qDebug() << vdv;
    if (!abstract.save_csv(fs)) {
        qDebug("打开失败");
    }
}