//
// Created by jiang.wenqiang on 2018/9/19.
//

#include <QtCore/QDebug>
#include "Revolve.hpp"

Revolve::Revolve(Collect *collect, Transform *transform, Trigger *trigger) :
        _collect(collect), _transform(transform), _trigger(trigger)
{
    connect(_collect, &Collect::framesGot, this, &Revolve::collectFramesGot,
            Qt::DirectConnection);
}

void Revolve::collectFramesGot()
{
    if (!_transform->isRunning()) {
        _transform->start();
    }
}

void Revolve::run()
{
    _collect->start();
    while (_collect->isRunning() || _transform->isRunning()) {}
}
