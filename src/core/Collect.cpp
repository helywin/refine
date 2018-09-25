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
        _control(CollectControl::Resume) {}


void Collect::run()
{
    if (_manner == FromCan) {
        _can->clear();
        int cnt = 0;
        while (_control != Stop && _control != Interrupt) {
            msleep(_delay);
            if (_control == Stop) {
                break;
            } else if (_control == Suspend) {
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
                emit framesGot();
//                qDebug("采集到！");
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
        }
    } else if (_manner == FromFile) {
        QFile f;
        File file;
        file.loadFrameRecordBegin(f, nullptr);
        while (1) { file.loadFrameRecord(*_buffer); }
    } else {}

    emit collectionFinish();
}
