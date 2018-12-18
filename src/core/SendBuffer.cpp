//
// Created by jiang.wenqiang on 2018/12/18.
//

#include "SendBuffer.hpp"

SendBuffer::SendBuffer(int len) :
        _cells(new CanObj[len]),
        _len(len),
        _begin(0),
        _end(0) {}

SendBuffer::~SendBuffer()
{
    delete[](_cells);
}
