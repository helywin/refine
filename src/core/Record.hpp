//
// Created by jiang.wenqiang on 2018/10/15.
//

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

public:
    Record() : _file(), _buffer(nullptr), _record(nullptr) {}

    bool beginRecord(QFile *record, Buffer *buffer);

    inline void finishRecord() { _file.dumpFrameRecordFinish(*_record); }

protected:
    void run() override;

signals:
    void error(int code);
};


#endif //REFINE_RECORD_HPP
