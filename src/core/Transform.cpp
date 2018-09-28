//
// Created by jiang.wenqiang on 2018/9/11.
//

#include <QtCore/QDebug>
#include "Transform.hpp"
#include "Buffer.hpp"
#include "Curve.hpp"
#include "Tribe.hpp"

Transform::Transform(Buffer *buffer, Curve *curve, Tribe *tribe,
                     QFile *file_frames, bool save_frames) :
        _buffer(buffer),
        _curve(curve),
        _tribe(tribe),
        _file_frames(file_frames),
        _frames_stored(save_frames),
        _file() {}

void Transform::run()
{
    _buffer->setMark();
    if (_frames_stored) {
        qDebug() << "1: head:" << _buffer->headMarked() << " tail:" << _buffer->tailMarked();
        _file.dumpFrameRecord(*_buffer);
    }
    qDebug() << "2: head:" << _buffer->headMarked() << " tail:" << _buffer->tailMarked();
    for (const auto &buf : (*_buffer)) {
//        qDebug() << buf.str();
        qDebug() << buf.index();
        for (int i = 0; i < buf.dataSize(); ++i) {
//            unsigned short index = 0;
            float result = 0;
//            qDebug() << i;
            for (const auto &cur : *_curve) {
//                qDebug() << cur.str();
                bool flag = (cur.canId() == buf[i]->ID) &&
                            (cur.zeroByte() == -1 ||
                             buf[i]->Data[0] == cur.zeroByte());
                if (flag) {
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
////            qDebug() << "low byte " << low_byte;
                    low_byte <<= 7 - cur.lowByteRange()[1];
                    low_byte >>= 7 - cur.lowByteRange()[1] +
                                 cur.lowByteRange()[0];
                    full = high_byte + low_byte;
                    float k;
                    float b;
                    k = (float) (cur.rangeOut()[1] - cur.rangeOut()[0]) /
                        (float) (cur.rangeIn()[1] - cur.rangeIn()[0]);
                    b = (float) cur.rangeOut()[0] - k * cur.rangeIn()[0];
                    result = full * k + b;
//                    qDebug() << result;
                    (*_tribe)[cur.name()].data().append(result);
                }
            }
        }
    }
    _buffer->closeMark();
}
