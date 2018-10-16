//
// Created by jiang.wenqiang on 2018/10/15.
//

#include "Transform.hpp"
#include "Buffer.hpp"
#include "Curve.hpp"
#include "Tribe.hpp"

Transform::Transform() :
        _curve(nullptr),
        _buffer(nullptr),
        _tribe(nullptr),
        _file(){}

void Transform::beginTransform(Curve *curve, Buffer *buffer, Tribe *tribe)
{
    _curve = curve;
    _buffer = buffer;
    _tribe = tribe;
}

void Transform::run()
{
    _tribe->setUnFilled();
    for (const auto &buf : *_buffer) {
        for (int i = 0; i < buf.dataSize(); ++i) {
            QList<int> index_list;
            if (buf[i]->ID == 0x777) {
                index_list = _curve->findMap777(buf[i]->Data[0]);
            } else {
                index_list = _curve->findOtherMap(buf[i]->ID);
            }
            for (const auto index : index_list) {
                if ((*_tribe)[index].fill()) {
                    continue;   //同一曲线多的数据丢弃
                }
                auto cur = (*_curve)[index];
                unsigned int full;
                unsigned int high_byte;
                unsigned int low_byte;
                if (cur.highByte() != -1) {
                    high_byte = buf[i]->Data[cur.highByte()];
                    high_byte <<= 7 - cur.highByteRange()[1];
                    high_byte >>= 7 - cur.highByteRange()[1] +
                                  cur.highByteRange()[0];
                    high_byte <<= cur.lowByteRange()[1] -
                                  cur.lowByteRange()[0] + 1;
                } else {
                    high_byte = 0;
                }
                low_byte = buf[i]->Data[cur.lowByte()];
                low_byte <<= 7 - cur.lowByteRange()[1];
                low_byte >>= 7 - cur.lowByteRange()[1] +
                             cur.lowByteRange()[0];
                full = high_byte + low_byte;
                float k;
                float b;
                k = (float) (cur.rangeOut()[1] -
                             cur.rangeOut()[0]) /
                    (float) (cur.rangeIn()[1] - cur.rangeIn()[0]);
                b = (float) cur.rangeOut()[0] -
                    k * cur.rangeIn()[0];
                float result = full * k + b;
                (*_tribe)[cur.name()].push(Tribe::Data, result);
            }
        }
    }

    //补充没有的报文
    for (auto &iter : *_tribe) {
        if (!iter.fill()) {
            if (iter.empty()) {
                iter.push(Tribe::FakeByZero, 0);
            } else {
                iter.push(Tribe::FakeByPrevious, iter.data().last());
            }
        }
    }
}

void Transform::finishTransform(QFile *file)
{

}
