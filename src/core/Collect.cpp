//
// Created by jiang.wenqiang on 2018/9/11.
//

#include "Collect.hpp"
#include "Can.hpp"
#include "File.hpp"
#include "Buffer.hpp"

Collect::Collect(Can *can, Buffer *buffer) :
        _can(can),
        _buffer(buffer),
        _manner(CollectManner::FromCan),
        _delay(10),
        _file(nullptr),
        _control(CollectControl::Resume),
        _tribe(nullptr),
        _curve(nullptr),
        _single_thread(false) {}


void Collect::run()
{
    if (_manner == FromCan) {
        _can->clear();
        int cnt = 0;
        int cnt1 = 0;
        while (_control != Stop && _control != Interrupt) {
            if (_control == Suspend) {
                msleep(_delay);
                _can->clear();
                continue;
            }
            if (_buffer->isFull()) {
                emit error(BufferFull);
                qCritical("缓冲区满了！");
            }
            int flag = _can->collect(*_buffer);
            if (flag == Can::ReceiveFailed) {
                emit error(CanFailed);
                qCritical("CAN没连接上！");
            } else if(flag == Can::ReceiveSucceededWithFrames) {
//                if (_single_thread) {
//                    for (const auto &buf : (*_buffer)) {
////#pragma omp parallel for
//                        for (int i = 0; i < buf.dataSize(); ++i) {
////            unsigned short index = 0;
//                            float result = 0;
////            qDebug() << i;
//                            for (const auto &cur : *_curve) {
////                qDebug() << cur.str();
//                                bool flag_m = (cur.canId() == buf[i]->ID) &&
//                                            (cur.zeroByte() == -1 ||
//                                             buf[i]->Data[0] == cur.zeroByte());
//                                if (flag_m) {
//                                    unsigned int full;
//                                    unsigned int high_byte;
//                                    unsigned int low_byte;
//                                    if (cur.highByte() != -1) {
//                                        high_byte = buf[i]->Data[cur.highByte()];
//                                        high_byte <<= 7 - cur.highByteRange()[1];
//                                        high_byte >>= 7 - cur.highByteRange()[1] +
//                                                      cur.highByteRange()[0];
//                                        high_byte <<= cur.lowByteRange()[1] -
//                                                      cur.lowByteRange()[0] + 1;
//                                    } else {
//                                        high_byte = 0;
//                                    }
//                                    low_byte = buf[i]->Data[cur.lowByte()];
//                                    low_byte <<= 7 - cur.lowByteRange()[1];
//                                    low_byte >>= 7 - cur.lowByteRange()[1] +
//                                                 cur.lowByteRange()[0];
//                                    full = high_byte + low_byte;
//                                    float k;
//                                    float b;
//                                    k = (float) (cur.rangeOut()[1] - cur.rangeOut()[0]) /
//                                        (float) (cur.rangeIn()[1] - cur.rangeIn()[0]);
//                                    b = (float) cur.rangeOut()[0] - k * cur.rangeIn()[0];
//                                    result = full * k + b;
//                                    (*_tribe)[cur.name()].data().append(result);
//                                }
//                            }
//                        }
//                    }
//                } else {
                    cnt1 += 1;
                    cnt1 %= 25;
                    if (cnt1 == 24) {
                        emit framesGot();
                    }
//                }
////                qDebug("采集到！");
            } else {
                cnt += 1;
                if (cnt == 100) {
                    cnt = 0;
                    if (!_can->isConnected()) {
                        emit error(ConnectionLost);
                        qCritical("CAN没连接上！");
                        break;
                    }
                }
            }
            msleep(_delay);
        }
    } else if (_manner == FromFile) {
        qDebug("从文件采集");
        File file;
        file.loadFrameRecordBegin(*_file, *_buffer, nullptr, nullptr);
        while (_control != Stop && _control != Interrupt) {
            if (_control == Suspend) {
                continue;
            }
            if (!file.loadFrameRecord(*_buffer)) {
                break;
            } else {
//                qDebug("a frame");
                emit framesGot();
            }
            msleep(_delay);
//            msleep(0);
        }
        file.loadFrameRecordFinish(*_file);
    } else {}

    emit collectionFinish();
}
