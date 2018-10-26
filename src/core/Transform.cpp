/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/15
 * @brief 报文转曲线
 * @details 报文根据曲线配置转换成曲线数据，同时补充丢失曲线数据
 ******************************************************************************/

#include "Transform.hpp"
#include "Curve.hpp"
#include "Tribe.hpp"

Transform::Transform() :
        _curve(nullptr),
        _buffer(nullptr),
        _tribe(nullptr),
        _file() {}

void Transform::setParams(Curve *curve, Buffer *buffer, Tribe *tribe)
{
    _curve = curve;
    _buffer = buffer;
    _tribe = tribe;
}

void Transform::run()
{
#ifdef way0
    _tribe->setUnFilled();
    Buffer::Cell &buf = *_buffer->last();
    if (_buffer->isEmpty()) {
        return;
    }
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
#else
    _tribe->setUnFilled();
    Buffer::Cell &buf = *_buffer->last();
    if (_buffer->isEmpty()) {
        return;
    }
    for (Tribe::Cell &tr : *_tribe) {
        const Curve::Cell &cur = (*_curve)[tr.name()];
        for (int i = 0; (i < buf.dataSize()) && !tr.fill(); ++i) {
            if (cur.canId() == buf[i]->ID &&
            (cur.zeroByte() == -1 || cur.zeroByte() == buf[i]->Data[0])) {
                unsigned int high_byte = 0;
                unsigned int low_byte = 0;
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
                unsigned int full = high_byte + low_byte;
                float k;
                float b;
                k = (float) (cur.rangeOut()[1] -
                             cur.rangeOut()[0]) /
                    (float) (cur.rangeIn()[1] - cur.rangeIn()[0]);
                b = (float) cur.rangeOut()[0] -
                    k * cur.rangeIn()[0];
                float result = full * k + b;
                tr.push(Tribe::Data, result);
            }
        }
        if (!tr.fill()) {
            if (tr.empty()) {
                tr.push(Tribe::FakeByZero, 0);
            } else {
                tr.push(Tribe::FakeByPrevious, tr.data().last());
            }
        }
    }
#endif
}

void Transform::finish(QFile *file)
{
    if (file != nullptr) {
        _file.dumpCurveRecord(*file, *_tribe);
    }
}
