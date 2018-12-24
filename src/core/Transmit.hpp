/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/16
 * @brief 发送报文
 * @details 模拟采集时接收的报文，发送到CAN总线上
 ******************************************************************************/

#ifndef REFINE_TRANSMIT_HPP
#define REFINE_TRANSMIT_HPP

#include <QtCore/QThread>
#include "Message.hpp"
#include "Global.hpp"

class SendBuffer;
class Can;

class Transmit : public QThread, public Message
{
Q_OBJECT
public:

private:
    SendBuffer *_buffer;
    Can *_can;
    unsigned long _msec;
    int _frame_num;
    Re::RunningCommand _command;
    Re::RunningStatus _status;

public:
    explicit Transmit(Message *message = nullptr);
    void setParams(Can *can, SendBuffer *buffer, unsigned long msec, int frame_num);

    inline void begin()
    {
        _status = Re::Running;
        start();
    }

    void stop()
    {
        _command = Re::CommandStop;
        while (isRunning()) {}
    }

    inline void setMsec(unsigned long msec) { _msec = msec; }

    inline void setFrames(int frame_num) { _frame_num = frame_num; }

protected:
    void run() override;

signals:
    void progress(double value);

};


#endif //REFINE_TRANSMIT_HPP
