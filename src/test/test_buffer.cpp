//
// Created by jiang.wenqiang on 2018/9/28.
//

#include <QtCore/QDebug>
#include "RecvBuffer.hpp"

int main()
{
    RecvBuffer buffer;
    buffer.move();
    buffer.setMark();
    for (const auto &iter : buffer) {
        qDebug() << iter.str();
    }
}