//
// Created by jiang.wenqiang on 2018/7/6.
//

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include <QtCore/QTime>

#include "Log.h"

int main() {
    QTime t;
    t.start();
    qInstallMessageHandler(Log::handler);
    Log::set_path(QString("D:/jiang.wenqiang/code/autophi/core/log.txt"));
    qInfo("===============启动测试采集==============");
    QFile f(QString("/10.8.14.3/div17/部门文件夹/电控研究部/交换数据/测试科/公共文件夹/共享数据/j姜文强"));
    f.open(QIODevice::ReadOnly);
    if (!f.isOpen()) {
        qWarning("打不开");
        f.close();
    }
    QTextStream stream(&f);
    QString s;
    while (!stream.atEnd()) {
        s = stream.readLine(500);
        qDebug() << s;
    }
    f.close();
    qDebug() << t.elapsed();
    return 0;
}