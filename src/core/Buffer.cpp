//
// Created by jiang.wenqiang on 2018/6/29.
//

#include "Buffer.h"
#include "Log.h"

//Cell::Cell() = default {}

void Buffer::Cell::init(unsigned long size, int delay) {
    this->_size = size;
    this->_delay = delay;
    this->_index = 0;
    this->_length = 0;
    _buffer = new VCI_CAN_OBJ[size];
}

Buffer::Cell::~Cell() {
    delete[] _buffer;
}

PVCI_CAN_OBJ Buffer::Cell::operator[](unsigned int index) {
    if (index < _size) {
        return _buffer + index;
    } else {
        return nullptr;
    }
}

void Buffer::Cell::str(QStringList &list) {
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

QString Buffer::Cell::header(){
    return QString("  地 址  \t  时 间  \t长度\t   数 据   ");
}

Buffer::Buffer(unsigned int length, unsigned int size) :
        length(length), index(0) {
    buffer_list = new Cell[length];
    for (unsigned int i = 0; i < length; ++i) {
        buffer_list[i].init(size, -1);
    }
    head_point = buffer_list;
    tail_point = buffer_list;
}

Buffer::~Buffer() {
    delete[] buffer_list;
}

void Buffer::inc() {
    head_point->_index = this->index;
    if (head_point == buffer_list + length - 1) {
        head_point = buffer_list;
    } else {
        head_point += 1;
    }
}

void Buffer::dec() {
    if (tail_point == buffer_list + length - 1) {
        tail_point = buffer_list;
    } else {
        tail_point += 1;
    }
}

Buffer::Cell *Buffer::out() {
    if (empty()) {
        return nullptr;
    } else {
        Cell *p;
        p = tail_point;
        dec();
        return p;
    }
}

Buffer::Cell *Buffer::push() {
    if (full()) {
        return nullptr;
    } else {
        Cell *p;
        p = head_point;
        inc();
        return p;
    }
}

bool Buffer::empty() {
    return head_point == tail_point;
}

<<<<<<< HEAD
bool Buffer::full() {
    return head_point - tail_point == length - 1 ||
           head_point - tail_point == -1;
=======
//inline void Buffer::Cell::setDelay(const int delay) {
//    _delay = delay;
//}

//void Buffer::Cell::clear() {
//    _status = Status::UnUsed;
//}

//inline int Buffer::Cell::delay() const {
//    return _delay;
//}

QStringList Buffer::Cell::str() const {
    QStringList list;
    for (unsigned int i = 0; i < _data_size; ++i) {
        QString str;
        str = QString("0x%1 ").arg(_cell[i].ID, 8, 16, QChar('0'));
        str += QString("%1 ").arg(_cell[i].TimeStamp / 1000, 0, 10);
        str += QString::number(_cell[i].DataLen) + " ";
        str += QString("0x%1 %2 %3 %4 %5 %6 %7 %8")
                .arg(_cell[i].Data[0], 2, 16, QChar('0'))
                .arg(_cell[i].Data[1], 2, 16, QChar('0'))
                .arg(_cell[i].Data[2], 2, 16, QChar('0'))
                .arg(_cell[i].Data[3], 2, 16, QChar('0'))
                .arg(_cell[i].Data[4], 2, 16, QChar('0'))
                .arg(_cell[i].Data[5], 2, 16, QChar('0'))
                .arg(_cell[i].Data[6], 2, 16, QChar('0'))
                .arg(_cell[i].Data[7], 2, 16, QChar('0'));
        list.append(str);
    }
    return qMove(list);
>>>>>>> 07d1fd7... 发送数据小工具写完,还需测试
}

Buffer::Cell *Buffer::head() {
    return head_point;
}

Buffer::Cell *Buffer::tail() {
    return tail_point;
}