/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/15
 * @brief 报文存储
 * @details 采集数据时调用报文存储线程来存储报文
 ******************************************************************************/

#include "Record.hpp"


void Record::run()
{
    _buffer_head = _buffer->end();
    _file.dumpFrameRecord(*_buffer, _buffer_tail, _buffer_head);
}
