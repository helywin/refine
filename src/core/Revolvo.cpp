//
// Created by jiang.wenqiang on 2018/8/13.
//

#include "Revolvo.h"

Revolvo::Revolvo(Transmit *transmit, Transforo *transforo,
                 int limit, int interval) :
        _transmit(transmit), _transforo(transforo), _time_limit(limit),
        _interval(interval) {
    Q_ASSERT(transmit != nullptr && transforo != nullptr);
    Q_ASSERT(limit > 0);
    Q_ASSERT(interval > 0);
    connect(&_timer, &QTimer::timeout, _transmit, &Transmit::start);
}

void Revolvo::setTimeLimit(const int limit) {
    Q_ASSERT(limit > 0);
    _time_limit = limit;
}

void Revolvo::setTransmit(Transmit *transmit) {
    Q_ASSERT(transmit != nullptr);
    disconnect(&_timer, &QTimer::timeout, _transmit, &Transmit::start);
    _transmit = transmit;
    connect(&_timer, &QTimer::timeout, _transmit, &Transmit::start);
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
    _timer.start(_interval);
    while (_transmit->isRunning()) {}
}

void Revolvo::pulse() {
    _timer.stop();
}

void Revolvo::pulse(int msec) {
    Q_ASSERT(msec > 0);
}

void Revolvo::resume() {
    _timer.start(_interval);
}
