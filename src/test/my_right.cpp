//
// Created by jiang.wenqiang on 2018/7/17.
//

#include "my_right.h"

Right::Right(Right &&other) {
    qSwap(str, other.str);
    other.str = nullptr;
    qDebug() << "Right拷贝构造元素: " << str;
}

Right::Right(QString &&str) : str(str) {
    qDebug() << "Right拷贝构造字符串: " << str;
}

Right::~Right() {
    qDebug() << "Right析构函数: " << str;
}

Left::Left(const Left &other) : str(other.str) {
    qDebug() << "Left拷贝构造: " << str;
}