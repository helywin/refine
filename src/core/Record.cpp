/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/15
 * @brief 报文存储
 * @details 采集数据时调用报文存储线程来存储报文
 ******************************************************************************/

#include "Record.hpp"


Record::Record() :
        _file(),
        _buffer(nullptr),
        _record(nullptr),
        _buffer_tail(),
        _buffer_head(),
        _status(Stop),
        _cmd(CommandStop),
        _msec(10) {}

void Record::run()
{
    while (_cmd != CommandStop) {
        msleep(_msec);
        if (_cmd == CommandPause) {
            if (_status == Running) {
                _status = Pause;
            }
            continue;
        }
        if (_cmd == CommandResume &&
            _status == Pause) {
            _status = Running;
        }
        _buffer_head = _buffer->head();
        _file.dumpFrameRecord(*_buffer, _buffer_tail, _buffer_head);
        _buffer_tail = _buffer_head;
    }
}

void Record::begin()
{
    _cmd = None;
    if (_file.dumpFrameRecordBegin(*_record)) {
        start();
        _status = Running;
    }
}

void Record::stop()
{
    _cmd = CommandStop;
    while (isRunning()) {}
    _file.dumpFrameRecordFinish(*_record);
    _status = Stop;
}
