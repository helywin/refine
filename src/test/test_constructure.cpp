//
// Created by jiang.wenqiang on 2018/7/4.
//
#include "Log.h"

class A {
public:
    int num;
    char str[6];

    A() = default;

    A(const A &a) = default;

    A &operator=(const A &a) = default;
};

int main() {
    qInstallMessageHandler(Log::handler);
    Log::set_path(QString("D:/jiang.wenqiang/code/autophi/core/log.txt"));
    qInfo("===============启动构造函数==============");

    A a = A();
    a.num = 1;
    char str1[6] = "hello";
    char str2[6] = "world";
    for (int i = 0; i < 6; ++i) {
        a.str[i] = str1[i];
    }

//    A b = a;
    A b(a);
    b.num = 2;
    for (int i = 0; i < 6; ++i) {
        b.str[i] = str2[i];
    }

    qDebug() << a.str;
}