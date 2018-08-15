//
// Created by jiang.wenqiang on 2018/8/8.
//

#include "Revolvi.h"

Revolvi::Revolvi(Collect *collect, Transfori *transform,
                     int limit, int interval) :
        _collect(collect), _transform(transform), _time_limit(limit),
        _interval(interval) {
    Q_ASSERT(collect != nullptr && transform != nullptr);
    Q_ASSERT(limit > 0);
    Q_ASSERT(interval > 0);
    connect(&_timer, &QTimer::timeout, _collect, &Collect::start);
}

void Revolvi::setTimeLimit(const int limit) {
    Q_ASSERT(limit > 0);
    _time_limit = limit;
}

void Revolvi::setCollect(Collect *collect) {
    Q_ASSERT(collect != nullptr);
    _collect = collect;
}

void Revolvi::setTransfori(Transfori *transform) {
    Q_ASSERT(transform != nullptr);
    _transform = transform;
}

void Revolvi::setInterval(const int interval) {
    Q_ASSERT(interval > 0);
    _interval = interval;
}

void Revolvi::marvel() {
    _timer.start(_interval);
}

void Revolvi::pulse() {
    _timer.stop();
}

void Revolvi::pulse(int msec) {
    Q_ASSERT(msec > 0);
}

void Revolvi::resume() {
    _timer.start(_interval);
}

void Revolvi::collectResult(Collect::Result result) {
    if (result == Collect::Result::Succeeded) {
        _transform->start();
    }
}

