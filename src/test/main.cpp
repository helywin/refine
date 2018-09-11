//
// Created by jiang.wenqiang on 2018/9/10.
//

#include <QtCore/QDebug>

int main() {
    int a = 0x00;
    a |= 0x03;
    a ^= 0x01;
    qDebug() << a;
    return 0;
}