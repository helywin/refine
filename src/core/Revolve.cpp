//
// Created by jiang.wenqiang on 2018/10/15.
//

#include "Revolve.hpp"

Revolve::Revolve() :
        _can(),
        _curve(),
        _buffer(100),
        _collect(),
        _tribe(),
        _transform(&_curve, &_buffer, &_tribe),
        _record(),
        _revolve(),
        _softcan(),
        _timer(),
        _store_frames(),
        _collect_frames(),
        _store_curves(),
        _msec(),
        _is_transform(),
        _is_record()
{

}
