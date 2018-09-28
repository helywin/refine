//
// Created by jiang.wenqiang on 2018/9/28.
//

#include <QtCore/QDebug>
#include "Buffer.hpp"

int main()
{
    Buffer buffer;
    buffer.headForward();
    buffer.setMark();
    for (const auto &iter : buffer) {
        qDebug() << iter.str();
    }
}