//
// Created by jiang.wenqiang on 2018/9/19.
//

#include "Revolve.hpp"

Revolve::Revolve(Collect *collect, Transform *transform, Trigger *trigger) :
_collect(collect), _transform(transform), _trigger(trigger) {}

void Revolve::collectFramesGot()
{
    if (!_transform->isRunning()) {
        _transform->start();
    }
}
