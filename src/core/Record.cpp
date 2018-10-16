//
// Created by jiang.wenqiang on 2018/10/15.
//

#include "Record.hpp"

bool Record::beginRecord(QFile *record, Buffer *buffer)
{
    Q_ASSERT(record != nullptr);
    Q_ASSERT(buffer != nullptr);
    _record = record;
    _buffer = buffer;
    return _file.dumpFrameRecordBegin(*_record);
}

void Record::run()
{
    _file.dumpFrameRecord(*_buffer);
}
