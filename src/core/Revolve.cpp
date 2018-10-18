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
        _time(10)
{
    connect(&_timer_collect, &QTimer::timeout, this, &Revolve::tictoc,
            Qt::DirectConnection);
    connect(&_timer_stop, &QTimer::timeout, this, &Revolve::stop,
            Qt::DirectConnection);
    _collect.setParams(&_can, &_buffer, Collect::FromCan);
    _transform.setParams(&_curve, &_buffer, &_tribe);
    _record.setParams(_store_frames, &_buffer);
}

void Revolve::begin(int msec, int config, int time)
{
    if (_status != Stop) {
        return;
    }
    _msec = msec;
    _config.all = config;
    _time = time;
    _timer_collect.setInterval(_msec);
    _collect.begin();
    if (_config.bits.transform) {
        _transform.begin();
    }
    if (_config.bits.record) {
        _record.begin();
    }
    if (_config.bits.trigger) {
        _timer_stop.setInterval(_time * 1000);
        _timer_stop.start();
    }
    _timer_collect.start();
    _status = Running;
}

void Revolve::stop()
{
    if (_status == Stop) {
        return;
    }
    while (_collect.isRunning() ||
           _transform.isRunning() ||
           _record.isRunning()) {}
    _timer_collect.stop();
    if (_config.bits.timing) {
        _timer_stop.stop();
    }
    _collect.finish();
    _transform.finish(_store_curves);
    _record.finish();
    _status = Stop;
}

void Revolve::pause()
{
    if (_status != Running) {
        return;
    }
    _timer_collect.stop();
    if (_config.bits.timing) {
        _time = _timer_stop.remainingTime();
        _timer_stop.stop();
    }
}

void Revolve::resume()
{
    if (_status != Pause) {
        return;
    }
    _timer_collect.start();
    if (_config.bits.timing) {
        _time = _timer_stop.remainingTime();
        _timer_stop.stop();
    }
}

void Revolve::tictoc()
{
    _collect.start();
    if (_config.bits.transform) {
        _transform.start();
    }
    if (_config.bits.record) {
        _record.start();
    }
}

void Revolve::setCollectManner(Collect::Manner manner, QString &collect_frame)
{
    _collect_frames = new QFile(collect_frame);
    _collect.setParams(&_can, &_buffer, manner, _collect_frames);
}

void Revolve::genFramesDataFile()
{
    _store_frames->setFileName()
}

void Revolve::genCurveDataFile()
{

}
