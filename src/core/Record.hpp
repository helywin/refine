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
#include "Message.hpp"

/*!
 * @brief 用来存储报文的类
 */

class Record : public QThread, public Message
{
Q_OBJECT
public:
    enum Info
    {
        WarnFile
    };

    enum Status
    {
        Stop,
        Running,
        Pause
    };

    enum Command
    {
        None,
        CommandStop,
        CommandResume,
        CommandPause
    };
private:
    File _file;
    Buffer *_buffer;
    QFile *_record;
    Buffer::Iter _buffer_tail;
    Buffer::Iter _buffer_head;
    Status _status;
    Command _cmd;
    unsigned long _msec;

public:
    explicit Record(Message *message = nullptr);

    inline void setParams(QFile *record, Buffer *buffer)
    {
        _record = record;
        _buffer = buffer;
        _buffer_tail.setParams(_buffer, 0);
        _buffer_head.setParams(_buffer, 0);
    }

    void begin();

    void stop(bool error = false);

    inline void pause() { _cmd = CommandPause; }

    inline void reusme() { _cmd = CommandResume; }

    inline int status() const { return _status; }

protected:
    void run() override;

signals:
    void info(int code);
};


#endif //REFINE_RECORD_HPP
