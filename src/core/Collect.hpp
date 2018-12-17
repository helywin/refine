/*******************************************************************************
 * @file Revolve.cpp
 * @author jiang.wenqiang
 * @date 2018/10/15
 * @brief 底层调度
 * @details 底层功能模块全部由Revolve实现调度，GUI不参与任何变量初始化和传递
 ******************************************************************************/

#ifndef REFINE_COLLECT_HPP
#define REFINE_COLLECT_HPP

#include <QtCore/QThread>
#include <QtCore/QFile>
#include "Buffer.hpp"
#include "Can.hpp"
#include "File.hpp"
#include "Message.hpp"
#include "Global.hpp"

class Collect : public QThread, public Message
{
Q_OBJECT
public:
    enum Manner
    {
        FromCan,
        FromFile
    };

    enum Info
    {
        InfoFileEnd,
        WarnNoFrame,
        ErrorConnection,
        ErrorFile
    };

private:
    Can *_can;
    Buffer *_buffer;
    Manner _manner;
    QFile _frame_file;
    File _file;
    Re::RunningStatus _status;
    Re::RunningCommand _cmd;
    unsigned long _msec;
    int _frames_loop;

public:
    explicit Collect(Message *message = nullptr);

    void setParams(Can *can, Buffer *buffer,
                   Manner manner, unsigned long msec = 10,
                   const QString &name = QString());

    void begin();

    inline void pause() { _cmd = Re::CommandPause; }

    inline void resume() { _cmd = Re::CommandResume; }

    void stop();

    inline Manner manner() const { return _manner; }

    inline int status() const { return _status; }

protected:
    void run() override;

signals:
    void info(int code);

    void baudRate(double baud_rate);
};


#endif //REFINE_COLLECT_HPP
