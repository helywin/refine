//
// Created by jiang.wenqiang on 2018/7/4.
//

#include "Log.h"

int main() {
    qInstallMessageHandler(Log::handler);
    Log::set_path(QString("D:/jiang.wenqiang/code/autophi/core/log.txt"));
    qInfo("==============测试位运算==============");
    unsigned int a = 56;
    unsigned int b = 0xFFFFFFFF;
    unsigned int c = 0xFFFFFFFF;
    b = b << 3U & b >> 26U;
    qDebug() << a;
    qDebug() << b;
    qDebug() << (b & a);
    qDebug() << sizeof(int);
    qDebug() << sizeof(short);
    qDebug() << c;
    c <<= 8;
    c >>= 8;
    qDebug() << c;
}