/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/16
 * @brief 发送报文
 * @details 模拟采集时接收的报文，发送到CAN总线上
 ******************************************************************************/

#ifndef REFINE_TRANSMIT_HPP
#define REFINE_TRANSMIT_HPP

#include <QtCore/QObject>
#include "Message.hpp"


class Transmit : public QObject, public Message
{

};


#endif //REFINE_TRANSMIT_HPP
