//
// Created by jiang.wenqiang on 2018/8/13.
//

#include <Transforo.h>
#include "Transforo.h"


Transforo::Transforo(Curve *curve, Buffer *buffer, Tribe *tribe) :
        _curve(curve), _buffer(buffer), _tribe(tribe), _size(tribe->length()),
        _index(0) {
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
    _index = 0;
    _size = tribe->length();
}

void Transforo::run() {
    auto p = _buffer->headCell();
    for (const auto &iter : _tribe->rawIndex()) {
        double result = _tribe->raw(iter)[_index];
        const Curve::Cell &curve = _curve->at(iter);
        long id = curve.canId();
        int index = -1;
        for (auto i = 0; i < p->dataSize(); ++i) {
            if (id == p->at(i)->ID) {
                if (curve.zeroByteExisted() &&
                    curve.zeroByte() == p->at(i)->Data[0]) {
                    index = i;
                } else if (!curve.zeroByteExisted()) {
                    index = i;
                }
            }
        }
        if (index == -1) {
            p->setDataSize(p->dataSize() + 1);      //可能越界，但不判断
            index = (int)p->dataSize();
        }

    }
    _buffer->headForward();
}

