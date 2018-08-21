//
// Created by jiang.wenqiang on 2018/8/8.
//

#include "Collect.h"

Collect::Collect(Can *can, Buffer *buffer) :
        _can(can), _buffer(buffer) {
    Q_ASSERT(can != nullptr && buffer != nullptr);
}

void Collect::setCan(Can *can) {
    Q_ASSERT(can != nullptr);
    _can = can;
}

void Collect::setBuffer(Buffer *buffer) {
    Q_ASSERT(buffer != nullptr);
    _buffer = buffer;
}

void Collect::run() {
    bool flag;
    if (_buffer->isFull()) {
        emit result(Result::BufferFull);
        return;
    }
    flag = _can->collect(*_buffer);
    if (flag) {
        emit result(Result::Succeeded);
    } else {
        emit result(Result::CanError);
    }
}
