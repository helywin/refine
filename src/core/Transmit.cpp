//
// Created by jiang.wenqiang on 2018/8/8.
//

#include "Transmit.h"

Transmit::Transmit(Can *can, Buffer *buffer) :
        _can(can), _buffer(buffer) {
    Q_ASSERT(can != nullptr);
    Q_ASSERT(buffer != nullptr);
}

void Transmit::setCan(Can *can) {
    Q_ASSERT(can != nullptr);
    _can = can;
}

void Transmit::setBuffer(Buffer *buffer) {
    Q_ASSERT(buffer != nullptr);
    _buffer = buffer;
}

void Transmit::run() {
    bool flag;
    if (_buffer->isEmpty()) {
        emit result(Result::BufferEmpty);
        return;
    }
    flag = _can->deliver(*_buffer);
    if (flag) {
        emit result(Result::Succeeded);
    } else {
        emit result(Result::CanError);
    }
}
