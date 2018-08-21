//
// Created by jiang.wenqiang on 2018/8/13.
//

#include <QtCore/QDebug>
#include "Revolvo.h"

Revolvo::Revolvo(Transmit *transmit, Transforo *transforo,
                 int limit, int interval) :
        _transmit(transmit), _transforo(transforo), _time_limit(limit),
        _interval(interval) {
    Q_ASSERT(transmit != nullptr && transforo != nullptr);
    Q_ASSERT(limit > 0);
    Q_ASSERT(interval > 0);
}

void Revolvo::setTimeLimit(const int limit) {
    Q_ASSERT(limit > 0);
    _time_limit = limit;
}

void Revolvo::setTransmit(Transmit *transmit) {
    Q_ASSERT(transmit != nullptr);
    _transmit = transmit;
}

void Revolvo::setTransforo(Transforo *transforo) {
    Q_ASSERT(transforo != nullptr);
    _transforo = transforo;
}

void Revolvo::setInterval(const int interval) {
    Q_ASSERT(interval > 0);
    _interval = interval;
}

void Revolvo::marvel() {
    _transforo->start();
    _transmit->start();
    while (_transmit->isRunning()) {}
}

void Revolvo::pulse() {
}

void Revolvo::pulse(int msec) {
    Q_ASSERT(msec > 0);
}

void Revolvo::resume() {
}
