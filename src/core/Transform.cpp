//
// Created by jiang.wenqiang on 2018/9/11.
//

#include <QtCore/QDebug>
#include "Transform.hpp"
#include "Buffer.hpp"
#include "Curve.hpp"

Transform::Transform(Buffer *buffer, Curve *curve, Tribe *tribe,
                     QFile *file_frames, bool save_frames) :
        _buffer(buffer),
        _curve(curve),
        _tribe(tribe),
        _file_frames(file_frames),
        _frames_stored(save_frames),
        _file(),
        _finish_collect(true) {}

void Transform::run()
{
    _finish_collect = false;
    while (!_finish_collect || !_buffer->isEmpty()) {
        if (_buffer->isEmpty()) {
            msleep(10);
            continue;
        }
        if (_frames_stored) {
            _file.dumpFrameRecord(*_buffer);
        }
        _buffer->setMark();
//    for (auto &iter : *_tribe) {
//        iter.data().append(0);
//    }
//        Timer::tic();
        for (const auto &buf : (*_buffer)) {
//#pragma omp parallel for
            for (int i = 0; i < buf.dataSize(); ++i) {
//            unsigned short index = 0;
                float result = 0;
//            qDebug() << i;
                const Curve::SubIdMap &map = _curve->subIdMap777();
                if (buf[i]->ID == 0x777 &&
                    map.keys().contains(buf[i]->Data[0])) {
//                    qDebug() << "in sub";
                    for (const auto &index : map[buf[i]->Data[0]]) {
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
                        result = full * k + b;
                        (*_tribe)[cur.name()].data().append(result);
                    }
                } else {
                    for (const auto &cur : *_curve) {
//                qDebug() << cur.str();
                        bool flag = (cur.canId() == buf[i]->ID) &&
                                    (cur.zeroByte() == -1 ||
                                     buf[i]->Data[0] == cur.zeroByte());
                        if (flag) {
//                    qDebug() << "transform" << flag;
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
                            result = full * k + b;
                            (*_tribe)[cur.name()].data().append(result);
                        }
                    }
                }
            }
        }
//        Timer::toc();
        _buffer->closeMark();
    }
}
