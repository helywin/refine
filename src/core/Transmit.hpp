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

class RecvBuffer;
class Can;

class Transmit : public QThread, public Message
{
Q_OBJECT
public:

private:
    RecvBuffer *_buffer;
    Can *_can;

public:
    explicit Transmit(Message *message = nullptr);
    void setParams(Can *can, RecvBuffer *buffer);

protected:
    void run() override;

signals:
    void progress(double value);

};


#endif //REFINE_TRANSMIT_HPP
