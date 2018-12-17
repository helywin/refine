/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/15
 * @brief 报文存储
 * @details 采集数据时调用报文存储线程来存储报文
 ******************************************************************************/

#include "Record.hpp"


Record::Record(Message *message) :
        Message(message),
        _file(message),
        _buffer(nullptr),
        _record(),
        _buffer_tail(),
        _buffer_head(),
        _status(Re::Stop),
        _cmd(Re::CommandStop),
        _msec(10) {}

void Record::run()
{
    while (_cmd != Re::CommandStop) {
        msleep(_msec);
        if (_cmd == Re::CommandPause) {
            if (_status == Re::Running) {
                _status = Re::Pause;
            }
            continue;
        }
        if (_cmd == Re::CommandResume &&
            _status == Re::Pause) {
            _status = Re::Running;
        }
        _buffer_head = _buffer->head();
        _file.dumpFrameRecord(*_buffer, _buffer_tail, _buffer_head);
        _buffer_tail = _buffer_head;
    }
}

void Record::begin()
{
    _cmd = Re::NoCommand;
    if (_file.dumpFrameRecordBegin(_record)) {
        start();
        _status = Re::Running;
    }
}

void Record::stop(bool error)
{
    _cmd = Re::CommandStop;
    while (isRunning()) {}
    if (!error) {
        _file.dumpFrameRecordFinish(_record);
    }
    _status = Re::Stop;
}
