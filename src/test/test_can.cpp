//
// Created by jiang.wenqiang on 2018/6/29.
//

#include "Can.h"
#include "Log.h"
#include <QtCore/QCoreApplication>

int main(int argc, char *argv[]) {
    qInstallMessageHandler(Log::handler);
    Log::set_path(QString("log.txt"));
    qInfo("==============启动==============");
    CanConfig config;
    Can can = Can(config);
    CanBoardInfo info = CanBoardInfo();
    can.close();
    can.open();
    can.init();
    can.start();
    can.board_info(info);
    QString str;
    qDebug() << info.str();
    can.close();
    qDebug("hello");
    qInfo("hell");
    qWarning("haha");
    qCritical("hh");
    str = "我,是,中,国,人";
    QStringList list = str.split(QChar(','));
    for (const auto &iter : list) {
        if (iter == "我") {
            qInfo("我");
        }
    }
    str = list.join(QChar('.'));
    qInfo() << str;
//    qFatal("dd");
}
