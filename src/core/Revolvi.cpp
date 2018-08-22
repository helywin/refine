//
// Created by jiang.wenqiang on 2018/8/8.
//

#include "Revolvi.h"

Revolvi::Revolvi(Collect *collect, Transfori *transfori,
                 int limit, int interval) :
        _collect(collect), _transfori(transfori), _time_limit(limit),
        _interval(interval) {
    Q_ASSERT(collect != nullptr && transfori != nullptr);
    Q_ASSERT(limit > 0);
    Q_ASSERT(interval > 0);
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
    _transfori = transform;
}

void Revolvi::setInterval(const int interval) {
    Q_ASSERT(interval > 0);
    _interval = interval;
}

void Revolvi::marvel() {
    _collect->start();
    while (_collect->isRunning()) {}
}

void Revolvi::pulse() {
}

void Revolvi::pulse(int msec) {
    Q_ASSERT(msec > 0);
}

void Revolvi::resume() {
}

void Revolvi::collectResult(Collect::Result result) {
    if (result == Collect::Result::Succeeded) {
        _transfori->start();
    }
}

