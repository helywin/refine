/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/15
 * @brief 底层调度
 * @details 底层功能模块全部由Revolve实现调度，GUI不参与任何变量初始化和传递
 ******************************************************************************/

#include "Revolve.hpp"

Revolve::Revolve() :
        _can(),
        _curve(),
        _buffer(100),
        _collect(),
        _tribe(),
        _transform(),
        _record(),
        _softcan(),
        _timer_collect(),
        _timer_stop(),
        _store_frames(nullptr),
        _collect_frames(nullptr),
        _store_curves(nullptr),
        _msec(10),
        _time(10),
        _is_transform(true),
        _is_record(true)
{
    connect(&_timer_collect, &QTimer::timeout, this, &Revolve::tictoc,
            Qt::DirectConnection);
    connect(&_timer_stop, &QTimer::timeout, this, &Revolve::stop,
            Qt::DirectConnection);
    _collect.setParams(&_can, &_buffer, Collect::FromCan);
    _transform.setParams(&_curve, &_buffer, &_tribe);
    _record.setParams(_store_frames, &_buffer);
}

void Revolve::begin(int msec, bool is_transform, bool is_record, int time)
{
    if (_timer_collect.isActive()) {
        return;
    }
    _msec = msec;
    _time = time;
    _timer_collect.setInterval(_msec);
    _collect.begin();
    if (_is_transform) {
        _transform.begin();
    }
    if (_is_record) {
        _record.begin();
    }
    if (_time) {
        _timer_stop.setInterval(time * 1000);
    }
    _timer_collect.start();
    _timer_stop.start();
}

void Revolve::stop()
{
    if (_timer_collect.isActive()) {
        while (_collect.isRunning() ||
               _transform.isRunning() ||
               _record.isRunning()) {}
        _timer_collect.stop();
        if (_timer_stop.isActive()) {
            _timer_stop.stop();
        }
        _collect.finish();
        _transform.finish(_store_curves);
        _record.finish();
    }
}

void Revolve::tictoc()
{
    _collect.start();
    if (_is_transform) {
        _transform.start();
    }
    if (_is_record) {
        _record.start();
    }
}
