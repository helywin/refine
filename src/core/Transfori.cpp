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
        return;
    }
    //todo 一个报文几条曲线的情况没有考虑，得重新写逻辑
    while (const Buffer::BufferType c = _buffer->tailCell()) {
        const Buffer::Cell &cell = *c;
        for (unsigned int i = 0; i < cell.dataSize(); ++i) {
            unsigned short index = 0;
            double result;
            for (int j = 0; j < _curve->size(); ++j) {
                const Curve::Cell &iter = (*_curve)[j];
                bool flag = iter.canId() == cell[i]->ID &&
                            (!iter.zeroByteExisted() ||
                             cell[i]->Data[0] == iter.zeroByte());
                if (flag) {
                    unsigned int full;
                    unsigned int high_byte;
                    unsigned int low_byte;
                    if (iter.highByteExisted()) {
                        high_byte = cell[i]->Data[iter.highByte()];
                        high_byte <<= 7 - iter.highByteRange()[1];
                        high_byte >>= 7 - iter.highByteRange()[1] +
                                      iter.highByteRange()[0];
                        high_byte <<= iter.lowByteRange()[1] -
                                      iter.lowByteRange()[0] + 1;
                    } else {
                        high_byte = 0;
                    }
                    low_byte = cell[i]->Data[iter.lowByte()];
//            qDebug() << "low byte " << low_byte;
                    low_byte <<= 7 - iter.lowByteRange()[1];
                    low_byte >>= 7 - iter.lowByteRange()[1] +
                                 iter.lowByteRange()[0];
                    full = high_byte + low_byte;
//            qDebug() << full;
                    double k;
                    double b;
                    k = (double) (iter.rangeOut()[1] - iter.rangeOut()[0]) /
                        (double) (iter.rangeIn()[1] - iter.rangeIn()[0]);
                    b = (double) iter.rangeOut()[0] - k * iter.rangeIn()[0];
                    result = (double) full * k + b;
                    index = iter.index();
                    _kebab->push(index, result);
                }
            }

        }
    }
}




