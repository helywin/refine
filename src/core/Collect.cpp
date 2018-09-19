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
        int cnt = 0;
        while (_control != Stop && _control != Interrupt) {
            msleep(_delay);
            cnt += 1;
            if (_control == Stop) {
                break;
            } else if (_control == Suspend) {
                continue;
            }
            if (cnt == 10) {
                cnt = 0;
                if (!_can->isConnected()) {
                    emit error(ConnectionLost);
                }
            }
            if (_buffer->isFull()) {
                emit error(BufferFull);
            }
            if (!_can->collect(*_buffer)) {
                emit error(CanFailed);
            } else {
                emit framesGot();
            }
        }
    } else if (_manner == FromFile) {
        QFile f;
        File file;
        file.loadFrameRecordBegin(f);
        while (1) { file.loadFrameRecord(*_buffer); }
    } else {}

    emit collectionFinish();
}
