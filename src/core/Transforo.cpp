//
// Created by jiang.wenqiang on 2018/8/13.
//

#include <QtCore/QDebug>
#include <Transforo.h>


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
    while (_index < _tribe->minLen()) {
        while (_buffer->isFull()) {
            msleep(10);
        }
        auto p = _buffer->headCell();
        for (const auto &iter : _tribe->rawIndex()) {
            double result = _tribe->raw(iter)[_index];
            const Curve::Cell &curve = _curve->at(iter);
            unsigned long id = curve.canId();
            int index = -1;
            for (unsigned i = 0; i < p->dataSize(); ++i) {
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
                index = (int) p->dataSize();
                p->setDataSize(p->dataSize() + 1);      //可能越界，但不判断

                p->at(index)->ID = curve.canId();
                p->at(index)->TimeStamp = 0;
                p->at(index)->TimeFlag = 0;
                p->at(index)->SendType = 2;        //! \brief 可以设置为自发自收
                p->at(index)->RemoteFlag = 0;
                p->at(index)->ExternFlag = 0;
                p->at(index)->DataLen = 8;
                if (curve.zeroByteExisted()) {
                    p->at(index)->Data[0] = (unsigned char) curve.zeroByte();
                }
            }
            double k;
            double b;
            k = (double) (curve.rangeIn()[1] - curve.rangeIn()[0]) /
                (double) (curve.rangeOut()[1] - curve.rangeOut()[0]);
            b = (double) curve.rangeIn()[0] - k * curve.rangeOut()[0];
            auto full = (long) (result * k + b);
            int high_length
                    = curve.highByteExisted() *
                      (curve.highByteRange()[1] - curve.highByteRange()[0] + 1);
            int low_length
                    = (curve.lowByteRange()[1] - curve.lowByteRange()[0] + 1);
            auto low_byte =
                    (unsigned char) (full % (1 << (unsigned) low_length));
            auto high_byte =
                    (unsigned char) (full / (1 << (unsigned) low_length));
            if (curve.highByteExisted()) {
                p->at(index)->Data[curve.highByte()] |=
                        high_byte << curve.highByteRange()[0];
            }
            p->at(index)->Data[curve.lowByte()] |=
                    low_byte << curve.lowByteRange()[0];
        }
        _index += 1;
        _buffer->headCell()->setSendType(
                Buffer::Cell::SendType::SelfSendRecieve);
        _buffer->headForward();
    }
}

void Transforo::resetIndex() {
    _index = 0;
}

