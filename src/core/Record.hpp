/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/15
 * @brief 报文存储
 * @details 采集数据时调用报文存储线程来存储报文
 ******************************************************************************/

#ifndef REFINE_RECORD_HPP
#define REFINE_RECORD_HPP

#include <QtCore/QThread>
#include <QtCore/QFile>
#include "File.hpp"
#include "Buffer.hpp"

/*!
 * @brief 用来存储报文的类
 */

class Record : public QThread
{
Q_OBJECT
public:
    enum Error
    {

    };
private:
    File _file;
    Buffer *_buffer;
    QFile *_record;
    Buffer::Iter _buffer_tail;
    Buffer::Iter _buffer_head;

public:
    Record() :
            _file(), _buffer(nullptr), _record(nullptr),
            _buffer_tail(), _buffer_head() {}

    inline void setParams(QFile *record, Buffer *buffer)
    {
        _record = record;
        _buffer = buffer;
        _buffer_tail.setParams(_buffer, 0);
        _buffer_head.setParams(_buffer, 0);
    }

    inline bool begin() { return _file.dumpFrameRecordBegin(*_record); }

    inline bool begin(QFile *record, Buffer *buffer)
    {
        setParams(record, buffer);
        return begin();
    }

    inline void finish() { _file.dumpFrameRecordFinish(*_record); }

protected:
    void run() override;

signals:
    void error(int code);
};


#endif //REFINE_RECORD_HPP
