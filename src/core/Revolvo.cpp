//
// Created by jiang.wenqiang on 2018/8/13.
//

#include "Revolvo.h"

Revolvo::Revolvo(Transmit *transmit, Transforo *transform,
                       int limit, int interval) :
        _transmit(transmit), _transform(transform), _time_limit(limit),
        _interval(interval) {
    Q_ASSERT(transmit != nullptr && transform != nullptr);
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
    _transmit = transmit;
}

void Revolvo::setTransforo(Transforo *transform) {
    Q_ASSERT(transform != nullptr);
    _transform = transform;
}

void Revolvo::setInterval(const int interval) {
    Q_ASSERT(interval > 0);
    _interval = interval;
}

void Revolvo::marvel() {
    _timer.start(_interval);
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

void Revolvo::collectResult(Transmit::Result result) {
    if (result == Transmit::Result::Succeeded) {
        _transform->start();
    }
}