/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/16
 * @brief 发送报文
 * @details 模拟采集时接收的报文，发送到CAN总线上
 ******************************************************************************/

#include "Transmit.hpp"
#include "Can.hpp"
#include "RecvBuffer.hpp"


Transmit::Transmit(Message *message) :
        Message(message),
        _buffer(nullptr),
        _can(nullptr)
{

}

void Transmit::setParams(Can *can, RecvBuffer *buffer)
{
    _can = can;
    _buffer = buffer;
}

void Transmit::run()
{
}
