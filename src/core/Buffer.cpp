//
// Created by jiang.wenqiang on 2018/9/10.
//

#include <QtCore/QDebug>
#include "Buffer.hpp"

Buffer::Cell::Cell() : _status(Status::UnInitialized), _objs(nullptr),
                       _whole_size(0), _data_size(0), _index(0) {}

Buffer::Cell::~Cell()
{
    if (_status != Status::UnInitialized) {
        delete[] _objs;
    }
}

void Buffer::Cell::initialize(unsigned int size)
{
    Q_ASSERT(size > 0);
    if (_status == Status::UnInitialized) {
        _objs = new VCI_CAN_OBJ[size];
        _whole_size = size;
        for (auto i = 0U; i < size; ++i) {
            memset(_objs[i].Data, 0, sizeof(_objs[i].Data));
        }
        _status = Status::Initialized;
    }
}

QStringList Buffer::Cell::str() const
{
    QStringList list;
    for (unsigned int i = 0; i < _data_size; ++i) {
        QString str;
        str = QString("0x%1 ").arg(_objs[i].ID, 8, 16, QChar('0'));
        str += QString("%1 ").arg(_objs[i].TimeStamp / 1000, 0, 10);
        str += QString::number(_objs[i].DataLen) + " ";
        str += QString("0x%1 %2 %3 %4 %5 %6 %7 %8")
                .arg(_objs[i].Data[0], 2, 16, QChar('0'))
                .arg(_objs[i].Data[1], 2, 16, QChar('0'))
                .arg(_objs[i].Data[2], 2, 16, QChar('0'))
                .arg(_objs[i].Data[3], 2, 16, QChar('0'))
                .arg(_objs[i].Data[4], 2, 16, QChar('0'))
                .arg(_objs[i].Data[5], 2, 16, QChar('0'))
                .arg(_objs[i].Data[6], 2, 16, QChar('0'))
                .arg(_objs[i].Data[7], 2, 16, QChar('0'));
        list.append(str);
    }
    return qMove(list);
}

void Buffer::Cell::setSendType(Buffer::Cell::SendType type)
{
    unsigned char send_type = 0;
    switch (type) {
        case SendType::Normal:
            break;
        case SendType::Once:
            send_type = 1;
            break;
        case SendType::SelfSendReceive:
            send_type = 2;
            break;
        case SendType::SelfSendReceiveOnce:
            send_type = 3;
            break;
    }
    for (auto i = 0U; i < dataSize(); ++i) {
        _objs[i].SendType = send_type;
    }
}

QDataStream &operator<<(QDataStream &stream, const Buffer::Cell &cell)
{
    int reserved = 0;
    stream << cell._index
           << cell._data_size
           << reserved
           << reserved;
    for (unsigned int i = 0; i < cell._data_size; ++i) {
        stream << cell._objs[i];
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Buffer::Cell &cell)
{
    int reserved;
    stream >> cell._index
           >> cell._data_size
           >> reserved
           >> reserved;
    for (unsigned int i = 0; i < cell._data_size; ++i) {
        stream >> cell._objs[i];
    }
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const VCI_CAN_OBJ &obj)
{
    stream << obj.ID
           << obj.TimeStamp
           << obj.TimeFlag
           << obj.SendType
           << obj.RemoteFlag
           << obj.ExternFlag
           << obj.DataLen;
    stream.writeRawData((const char *) obj.Data, 8);
    stream.writeRawData((const char *) obj.Reserved, 3);
    return stream;
}

QDataStream &operator>>(QDataStream &stream, VCI_CAN_OBJ &obj)
{
    stream >> obj.ID
           >> obj.TimeStamp
           >> obj.TimeFlag
           >> obj.SendType
           >> obj.RemoteFlag
           >> obj.ExternFlag
           >> obj.DataLen;
    stream.readRawData((char *) obj.Data, 8);
    stream.readRawData((char *) obj.Reserved, 3);
    return stream;
}


Buffer::Buffer(int cell_space, unsigned int cell_size) :
        _cell_space(cell_space), _index(0), _cells(new Cell[cell_space]),
        _head(0), _tail(0), _mark_head(0), _mark_tail(0), _read_cell(new Cell)
{
    Q_ASSERT(cell_space > 0);
    Q_ASSERT(cell_size > 0);
    for (int i = 0; i < _cell_space; ++i) {
        _cells[i].initialize(cell_size);
    }
    _read_cell->initialize(cell_size);
}


int Buffer::size() const
{
    auto i = _head - _tail;
    if (i < 0) {
        i += _cell_space;
    }
    return qMove(i);
}

void Buffer::size(int &cell_n, int &obj_n) const
{
    cell_n = 0;
    obj_n = 0;
    auto p = _tail;
    while (p != _head) {
        cell_n += 1;
        obj_n += (p + _cells)->dataSize();
        p += 1;
        p %= _cell_space;
    }
    qDebug() << obj_n;
}


void Buffer::headForward()
{
    (_head + _cells)->setIndex(_index);
    _index += 1;
    _head += 1;
    _head %= _cell_space;
}

void Buffer::tailForward()
{
    (_tail + _cells)->setDataSize(0);
    _tail += 1;
    _tail %= _cell_space;
}


QDataStream &operator<<(QDataStream &stream, const Buffer &buffer)
{
    auto p = buffer._mark_tail;
    while (p != buffer._mark_head) {
        stream << *(p + buffer._cells);
        p += 1;
        p %= buffer._cell_space;
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Buffer &buffer)
{
    stream >> *buffer._read_cell;
    return stream;
}



