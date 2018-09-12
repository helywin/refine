//
// Created by jiang.wenqiang on 2018/9/11.
//

#include "Collect.h"

Collect::Collect(Can *can, Buffer *buffer)
        : _can(can), _buffer(buffer),
          _manner(CollectFromCanWithDelay),
          _delay(10),
          _file(nullptr)
{

}

void Collect::setMode(const Collect::CollectManner manner, const int delay,
                      File *file)
{
    _manner = manner;
    _delay = delay;
    _file = file;
}

void Collect::setDelay(const int delay)
{
    Q_ASSERT(delay >= 0);
    _delay = delay;
}

void Collect::setFile(File *file)
{
    Q_ASSERT(file != nullptr);
    _file = file;
}

void Collect::run()
{
    QThread::run();
}
