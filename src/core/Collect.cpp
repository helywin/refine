//
// Created by jiang.wenqiang on 2018/9/11.
//

#include "Collect.hpp"
#include "Can.hpp"
#include "File.hpp"
#include "Buffer.hpp"

Collect::Collect(Can *can, Buffer *buffer)
        : _can(can), _buffer(buffer),
          _manner(FromCan),
          _delay(10),
          _file(nullptr),
          _control(Resume)
{

}

void
Collect::setMode(const Collect::CollectManner manner, const unsigned long delay,
                 File *file)
{
    _manner = manner;
    _delay = delay;
    _file = file;
}

void Collect::setDelay(const unsigned long delay)
{
    _delay = delay;
}

void Collect::setFile(File *file)
{
    Q_ASSERT(file != nullptr);
    _file = file;
}

void Collect::reset()
{

}

void Collect::startCollection()
{
    start(HighestPriority);
}

void Collect::suspendCollection()
{
    _control = Suspend;
}

void Collect::resumeCollection()
{
    _control = Resume;
}

void Collect::interruptCollection()
{
    _control = Interrupt;
}

void Collect::stopCollection()
{
    _control = Stop;
}

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
        _file->loadFrameRecordBegin(QFile());
    } else {}

    emit collectionFinish();
}
