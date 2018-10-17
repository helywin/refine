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
        _timer(),
        _store_frames(nullptr),
        _collect_frames(nullptr),
        _store_curves(nullptr),
        _msec(10),
        _is_transform(true),
        _is_record(true)
{
    connect(&_timer, &QTimer::timeout, this, &Revolve::tictoc,
            Qt::DirectConnection);
    _collect.setParams(&_can, &_buffer, Collect::FromCan);
    _transform.setParams(&_curve, &_buffer, &_tribe);
    _record.setParams(_store_frames, &_buffer);
}

void Revolve::marvel(int msec, bool is_transform, bool is_record)
{
    _msec = msec;
    _timer.setInterval(_msec);
    _collect.begin();
    if (is_transform) {
        _transform.begin();
    }
    if (is_record) {
        _record.begin();
    }
    _timer.start();
}

void Revolve::stop()
{
    while (_collect.isRunning() ||
           _transform.isRunning() ||
           _record.isRunning()) {}
    _timer.stop();
    _collect.finish();
    _transform.finish(_store_curves);
    _record.finish();
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
