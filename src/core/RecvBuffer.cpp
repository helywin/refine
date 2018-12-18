/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/10
 * @brief 帧缓冲区
 * @details 采集时缓存报文的帧缓冲区
 ******************************************************************************/

#include <QtCore/QDebug>
#include "RecvBuffer.hpp"

RecvBuffer::Cell::Cell() :  _objs(nullptr),
                       _whole_size(0), _data_size(0), _index(0) {}

RecvBuffer::Cell::~Cell()
{
    delete[] _objs;
}

void RecvBuffer::Cell::initialize(unsigned int size)
{
    Q_ASSERT(size > 0);
    _objs = new VCI_CAN_OBJ[size];
    _whole_size = size;
    for (auto i = 0U; i < size; ++i) {
        memset(_objs[i].Data, 0, sizeof(_objs[i].Data));
    }
}

QStringList RecvBuffer::Cell::str() const
{
    QStringList list;
    for (unsigned int i = 0; i < _data_size; ++i) {
        QString str;
        str = QString("0x%1 ").arg(_objs[i].ID, 3, 16, QChar('0'));
        str += QString("%1 ").arg(_objs[i].TimeStamp, 0, 10);
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
    return list;
}

void RecvBuffer::Cell::setSendType(Cd::SendType type)
{
    for (auto i = 0U; i < dataSize(); ++i) {
        _objs[i].SendType = type;
    }
}

QDataStream &operator<<(QDataStream &stream, const RecvBuffer::Cell &cell)
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

QDataStream &operator>>(QDataStream &stream, RecvBuffer::Cell &cell)
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

RecvBuffer::RecvBuffer(int cell_space, unsigned int cell_size) :
        _cell_space(cell_space), _index(0), _cells(new Cell[cell_space]),
        _head(0), _empty(true)
{
    Q_ASSERT(cell_space > 0);
    Q_ASSERT(cell_size > 0);
    for (int i = 0; i < _cell_space; ++i) {
        _cells[i].initialize(cell_size);
    }
}

RecvBuffer::~RecvBuffer()
{
    delete[] _cells;
}


void RecvBuffer::move()
{
    if (_empty) { _empty = false; }
    (_head + _cells)->setIndex(_index);
    _index += 1;
    _head += 1;
    _head %= _cell_space;
}

void RecvBuffer::dump(QDataStream &stream, RecvBuffer::Iter tail, RecvBuffer::Iter head)
{
    while (tail != head) {
        stream << *tail;
        ++tail;
    }
}

void RecvBuffer::reset()
{
    _index = 0;
    _head = 0;
    _empty = true;
}

void RecvBuffer::size(RecvBuffer::Iter tail, RecvBuffer::Iter head, unsigned int &packs,
                  unsigned int &frames)
{
    while (tail != head) {
        packs += 1;
        frames += (*tail).dataSize();
        ++tail;
    }
}



