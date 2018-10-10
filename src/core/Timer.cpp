//
// Created by jiang.wenqiang on 2018/10/10.
//

#include "Timer.hpp"

QTime Timer::_start = QTime(0, 0, 0,0);
QTime Timer::_end = QTime(0, 0, 0,0);

void Timer::tic()
{
    _start = QTime::currentTime();
}

void Timer::toc()
{
    _end = QTime::currentTime();
    qDebug() << "间隔(ms): " <<  _start.msecsTo(_end);
}
