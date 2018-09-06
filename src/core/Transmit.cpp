//
// Created by jiang.wenqiang on 2018/8/8.
//

#include "Transmit.h"

Transmit::Transmit(Can *can, Buffer *buffer) :
        _can(can), _buffer(buffer), _interval(10) , _cmd(Command::Resume) {
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

void Transmit::setInterval(unsigned long interval) {
    Q_ASSERT(interval > 0);
    _interval = interval;
}

void Transmit::pause() {
    _cmd = Command::Pause;
}

void Transmit::resume() {
    _cmd = Command::Resume;
}

void Transmit::stop() {
    _cmd = Command::Stop;
}

void Transmit::run() {
    while (true) {
//        qDebug(".");
        msleep(10);
        if (_cmd == Command::Stop) {
            return;
        } else if (_cmd == Command::Pause) {
            continue;
        }
        if (_buffer->isEmpty()) {
            emit result(Result::BufferEmpty);
            qDebug("empty");
            return;
        } else {
            bool flag;
            flag = _can->deliver(*_buffer);
            if (flag) {
                emit result(Result::Succeeded);
//                qDebug("succeed");
            } else {
                emit result(Result::CanError);
                qDebug("canerror");
            }
        }
    }
}

