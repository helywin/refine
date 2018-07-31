//
// Created by jiang.wenqiang on 2018/6/29.
//

#include "CanBuffer.h"
#include "Log.h"

//Cell::Cell() = default {}

void CanBuffer::Cell::init(unsigned long size, int delay) {
    this->_size = size;
    this->_delay = delay;
    this->_index = 0;
    this->_length = 0;
    _buffer = new VCI_CAN_OBJ[size];
}

CanBuffer::Cell::~Cell() {
    delete[] _buffer;
}

PVCI_CAN_OBJ CanBuffer::Cell::operator[](unsigned int index) {
    if (index < _size) {
        return _buffer + index;
    } else {
        return nullptr;
    }
}

void CanBuffer::Cell::str(QStringList &list) {
    list.clear();
    for (unsigned int i = 0; i < _length; ++i) {
        QString str;
        str = QString("0x%1\t").arg(_buffer[i].ID, 8, 16, QChar('0'));
        str += QString("%1\t").arg(_buffer[i].TimeStamp / 1000, 0, 10);
        str += QString::number(_buffer[i].DataLen) + "\t";
        str += QString("0x%1 %2 %3 %4 %5 %6 %7 %8")
                .arg(_buffer[i].Data[0], 2, 16, QChar('0'))
                .arg(_buffer[i].Data[1], 2, 16, QChar('0'))
                .arg(_buffer[i].Data[2], 2, 16, QChar('0'))
                .arg(_buffer[i].Data[3], 2, 16, QChar('0'))
                .arg(_buffer[i].Data[4], 2, 16, QChar('0'))
                .arg(_buffer[i].Data[5], 2, 16, QChar('0'))
                .arg(_buffer[i].Data[6], 2, 16, QChar('0'))
                .arg(_buffer[i].Data[7], 2, 16, QChar('0'));
        list.append(str);
    }
}

QString CanBuffer::Cell::header(){
    return QString("  地 址  \t  时 间  \t长度\t   数 据   ");
}

CanBuffer::CanBuffer(unsigned int length, unsigned int size) :
        length(length), index(0) {
    buffer_list = new Cell[length];
    for (unsigned int i = 0; i < length; ++i) {
        buffer_list[i].init(size, -1);
    }
    head_point = buffer_list;
    tail_point = buffer_list;
}

CanBuffer::~CanBuffer() {
    delete[] buffer_list;
}

void CanBuffer::inc() {
    head_point->_index = this->index;
    if (head_point == buffer_list + length - 1) {
        head_point = buffer_list;
    } else {
        head_point += 1;
    }
}

void CanBuffer::dec() {
    if (tail_point == buffer_list + length - 1) {
        tail_point = buffer_list;
    } else {
        tail_point += 1;
    }
}

CanBuffer::Cell *CanBuffer::out() {
    if (empty()) {
        return nullptr;
    } else {
        Cell *p;
        p = tail_point;
        dec();
        return p;
    }
}

CanBuffer::Cell *CanBuffer::push() {
    if (full()) {
        return nullptr;
    } else {
        Cell *p;
        p = head_point;
        inc();
        return p;
    }
}

bool CanBuffer::empty() {
    return head_point == tail_point;
}

bool CanBuffer::full() {
    return head_point - tail_point == length - 1 ||
           head_point - tail_point == -1;
}

CanBuffer::Cell *CanBuffer::head() {
    return head_point;
}

CanBuffer::Cell *CanBuffer::tail() {
    return tail_point;
}