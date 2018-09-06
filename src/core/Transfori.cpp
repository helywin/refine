//
// Created by jiang.wenqiang on 2018/8/8.
//

#include "Transfori.h"

Transfori::Transfori(Curve *curve, Buffer *buffer, Kebab *kebab) :
        _curve(curve), _buffer(buffer), _kebab(kebab) {
    Q_ASSERT(curve != nullptr);
    Q_ASSERT(buffer != nullptr);
    Q_ASSERT(kebab != nullptr);
}

void Transfori::setCurve(Curve *curve) {
    Q_ASSERT(curve != nullptr);
    _curve = curve;
}

void Transfori::setBuffer(Buffer *buffer) {
    Q_ASSERT(buffer != nullptr);
    _buffer = buffer;
}

void Transfori::setKebab(Kebab *kebab) {
    Q_ASSERT(kebab != nullptr);
    _kebab = kebab;
}

void Transfori::run() {
    if (_buffer->isEmpty()) {
        qDebug(".");
        return;
    }
    qDebug("tr");
    while (const Buffer::Cell * c = _buffer->tailCell()) {
        const Buffer::Cell &cell = *c;
        for (unsigned int i = 0; i < cell.dataSize(); ++i) {
            unsigned short index = 0;
            float_u result;
            for (int j = 0; j < _curve->size(); ++j) {
                const Curve::Cell &curve = (*_curve)[j];
                bool flag = curve.canId() == cell[i]->ID &&
                            (!curve.zeroByteExisted() ||
                             cell[i]->Data[0] == curve.zeroByte());
                if (flag) {
                    unsigned int full;
                    unsigned int high_byte;
                    unsigned int low_byte;
                    if (curve.highByteExisted()) {
                        high_byte = cell[i]->Data[curve.highByte()];
                        high_byte <<= 7 - curve.highByteRange()[1];
                        high_byte >>= 7 - curve.highByteRange()[1] +
                                      curve.highByteRange()[0];
                        high_byte <<= curve.lowByteRange()[1] -
                                      curve.lowByteRange()[0] + 1;
                    } else {
                        high_byte = 0;
                    }
                    low_byte = cell[i]->Data[curve.lowByte()];
//            qDebug() << "low byte " << low_byte;
                    low_byte <<= 7 - curve.lowByteRange()[1];
                    low_byte >>= 7 - curve.lowByteRange()[1] +
                                 curve.lowByteRange()[0];
                    full = high_byte + low_byte;
//            qDebug() << full;
                    float_u k;
                    float_u b;
                    k = (float_u) (curve.rangeOut()[1] - curve.rangeOut()[0]) /
                        (float_u) (curve.rangeIn()[1] - curve.rangeIn()[0]);
                    b = (float_u) curve.rangeOut()[0] - k * curve.rangeIn()[0];
                    result = (float_u) full * k + b;
                    index = curve.index();
                    _kebab->push(index, result);
                }
            }
        }
    }
}





