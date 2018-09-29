//
// Created by jiang.wenqiang on 2018/9/19.
//

#include <QtCore/QDebug>
#include "Revolve.hpp"

Revolve::Revolve(Collect *collect, Transform *transform, Trigger *trigger,
                 Tribe *tribe) :
        _collect(collect), _transform(transform), _trigger(trigger),
        _tribe(tribe)
{
    connect(_collect, &Collect::framesGot, this, &Revolve::collectFramesGot,
            Qt::DirectConnection);
}

void Revolve::collectFramesGot()
{
    if (!_transform->isRunning()) {
        _transform->start();
    } else {
        qDebug("busy");
    }
}

void Revolve::run()
{
    if (_collect->isRunning()) {
        qDebug("已经在采集！");
        return;
    }
    if (_transform->isFramesStored()) {
        _transform->initializeFramesStored();
    }
    reset();
    qDebug("开始采集");
    _collect->startCollection();
    while (_collect->isRunning() || _transform->isRunning()) {}
    if (_transform->isFramesStored()) {
        _transform->finishFramesStored();
    }
    qDebug("结束采集");
}

void Revolve::reset()
{
    _collect->reset();
    _transform->reset();
    _trigger->reset();
    _tribe->reset();
}
