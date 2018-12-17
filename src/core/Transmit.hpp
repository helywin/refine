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


class Transmit : public QThread, public Message
{
Q_OBJECT
public:
private:
public:
    explicit Transmit(Message *message = nullptr);

};


#endif //REFINE_TRANSMIT_HPP
