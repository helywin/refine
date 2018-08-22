//
// Created by jiang.wenqiang on 2018/8/8.
//

#include <QtCore/QDebug>
#include "Collect.h"

Collect::Collect(Can *can, Buffer *buffer) :
        _can(can), _buffer(buffer), _cmd(Command::Resume) {
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
    while (true) {
        msleep(10);
        qDebug("..");
        if(_cmd == Command::Stop) {
            return;
        } else if (_cmd == Command::Pause){
            continue;
        }
        if (_buffer->isFull()) {
            emit result(Result::BufferFull);
            qDebug("buffer full");
            return;
        } else {
            bool flag = _can->collect(*_buffer);
            if (flag) {
                emit result(Result::Succeeded);
                qDebug("succeeded");
            } else {
                emit result(Result::CanError);
                qDebug("can error");
            }
        }
    }
}
