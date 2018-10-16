//
// Created by jiang.wenqiang on 2018/10/15.
//

#include "Transform.hpp"
#include "Buffer.hpp"
#include "Curve.hpp"
#include "Tribe.hpp"

Transform::Transform(Curve *curve, Buffer *buffer, Tribe *tribe)
        : _curve(curve), _buffer(buffer), _tribe(tribe)
{
    Q_ASSERT(buffer != nullptr);
    Q_ASSERT(curve != nullptr);
    Q_ASSERT(tribe != nullptr);
}

void Transform::run()
{
    QThread::run();
}
