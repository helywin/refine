//
// Created by jiang.wenqiang on 2018/7/2.
//


#include "CurveConfig.h"
#include "Log.h"

int main() {
    qInstallMessageHandler(Log::handler);
    Log::setPath(QString("D:/jiang.wenqiang/code/autophi/core/log.txt"));
    qInfo("==============启动==============");
    QFile f("cfg.txt");
    CurveConfig curve;
    if (curve.load(f)) {
        QStringList list;
        curve.str(list);
        for (const auto &iter : list) {
            qInfo() << iter;
        }
    } else {
        qCritical("曲线配置载入出现问题");
    }
}