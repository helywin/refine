//
// Created by jiang.wenqiang on 2018/9/10.
//

#include <QtCore/QDebug>
#include "Version.hpp"

int main() {
    int a = 0x00;
    a |= 0x03;
    a ^= 0x01;
    qDebug() << a;
    qDebug() << Version::date();
    return 0;
}