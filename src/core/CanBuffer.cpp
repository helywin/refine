//
// Created by jiang.wenqiang on 2018/6/29.
//

#include "CanBuffer.h"
#include "Log.h"

//CanBufferCell::CanBufferCell() = default {}

void CanBufferCell::init(unsigned long size, int delay) {
    this->size = size;
    this->delay = delay;
    this->index = 0;
    this->l = 0;
    buffer = new VCI_CAN_OBJ[size];
}

CanBufferCell::~CanBufferCell() {
    delete[] buffer;
}

void CanBufferCell::clear() {
    index = 0;
    l = 0;
}

PVCI_CAN_OBJ CanBufferCell::operator[](unsigned int index) {
    if (index < size) {
        return buffer + index;
    } else {
        return nullptr;
    }
}

void CanBufferCell::str(QStringList &list) {
    list.clear();
    for (unsigned int i = 0; i < l; ++i) {
        QString str;
        str = QString("0x%1\t").arg(buffer[i].ID, 8, 16, QChar('0'));
        str += QString("%1\t").arg(buffer[i].TimeStamp / 1000, 0, 10);
        str += QString::number(buffer[i].DataLen) + "\t";
        str += QString("0x%1 %2 %3 %4 %5 %6 %7 %8")
                .arg(buffer[i].Data[0], 2, 16, QChar('0'))
                .arg(buffer[i].Data[1], 2, 16, QChar('0'))
                .arg(buffer[i].Data[2], 2, 16, QChar('0'))
                .arg(buffer[i].Data[3], 2, 16, QChar('0'))
                .arg(buffer[i].Data[4], 2, 16, QChar('0'))
                .arg(buffer[i].Data[5], 2, 16, QChar('0'))
                .arg(buffer[i].Data[6], 2, 16, QChar('0'))
                .arg(buffer[i].Data[7], 2, 16, QChar('0'));
        list.append(str);
    }
}

QString CanBufferCell::header() {
    return QString("  地 址  \t  时 间  \t长度\t   数 据   ");
}


CanBuffer::CanBuffer(unsigned int length, unsigned int size) :
        length(length), index(0) {
    buffer_list = new CanBufferCell[length];
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
    head_point->index = this->index;
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

CanBufferCell *CanBuffer::out() {
    if (empty()) {
        return nullptr;
    } else {
        CanBufferCell *p;
        p = tail_point;
        dec();
        return p;
    }
}

CanBufferCell* CanBuffer::push() {
    if (full()) {
        return nullptr;
    } else {
        CanBufferCell *p;
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

CanBufferCell* CanBuffer::head() {
    return head_point;
}

CanBufferCell* CanBuffer::tail() {
    return tail_point;
}