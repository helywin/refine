//
// Created by jiang.wenqiang on 2018/8/13.
//

#include <Transforo.h>
#include "Transforo.h"


Transforo::Transforo(Curve *curve, Buffer *buffer, Tribe *tribe) :
        _curve(curve), _buffer(buffer), _tribe(tribe) {
    Q_ASSERT(curve != nullptr);
    Q_ASSERT(buffer != nullptr);
    Q_ASSERT(tribe != nullptr);
}

void Transforo::setCurve(Curve *curve) {
    Q_ASSERT(curve != nullptr);
    _curve = curve;
}

void Transforo::setBuffer(Buffer *buffer) {
    Q_ASSERT(buffer != nullptr);
    _buffer = buffer;
}

void Transforo::setTribe(Tribe *tribe) {
    Q_ASSERT(tribe != nullptr);
    _tribe = tribe;
}

void Transforo::run() {
    QThread::run();
}

