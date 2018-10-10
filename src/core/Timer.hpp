//
// Created by jiang.wenqiang on 2018/10/10.
//

#ifndef REFINE_TIMER_HPP
#define REFINE_TIMER_HPP

#include <QtCore/QDebug>
#include <QtCore/QTime>

class Timer
{
private:
    static QTime _start;
    static QTime _end;

public:
    static void tic();
    static void toc();
};

//QTime Timer::_start = QTime(0, 0, 0,0);
//QTime Timer::_end = QTime(0, 0, 0,0);


#endif //REFINE_TIMER_HPP
