//
// Created by jiang.wenqiang on 2018/9/10.
//

#include "Buffer.hpp"

Buffer::Buffer() : Buffer(50, 100) {}

Buffer::Buffer(int cell_space, unsigned int cell_size) :
        _cell_space(cell_space), _index(0), _cells(new Cell[cell_space]),
        _head(_cells), _tail(_cells)
{
    Q_ASSERT(cell_space > 0);
    Q_ASSERT(cell_size > 0);
    for (int i = 0; i < _cell_space; ++i) {
        _cells[i].initialize(cell_size);
    }
}


Buffer::~Buffer()
{
    delete[] _cells;
}

Buffer::Cell *Buffer::operator[](int index)
{
    Q_ASSERT(index >= 0);
    return _cells + index;
}

const Buffer::Cell *Buffer::operator[](int index) const
{
    Q_ASSERT(index >= 0);
    return _cells + index;
}

int Buffer::space() const
{
    return _cell_space;
}

int Buffer::size() const
{
    auto p = _tail;
    int i = 0;
    while(p != _head) {
        if (p == _cells + _cell_space - 1) {
            p = _cells;
        } else {
            p += 1;
        }
        i += 1;
    }
    return i;
}

void Buffer::size(int &cell_n, int &obj_n) const
{
    cell_n = 0;
    obj_n = 0;
    auto p = _tail;
    while(p != _head) {
        if (p == _cells + _cell_space - 1) {
            p = _cells;
        } else {
            p += 1;
        }
        cell_n += 1;
        obj_n += p->dataSize();
    }
}

const VCI_CAN_OBJ *Buffer::head() const
{
    return _head->cell();
}


VCI_CAN_OBJ *Buffer::head()
{
    return _head->cell();
}

const VCI_CAN_OBJ *Buffer::tail() const
{
    return _tail->cell();
}

VCI_CAN_OBJ *Buffer::tail()
{
    return _tail->cell();
}

void Buffer::headForward()
{
    _head->setIndex(_index);
    _index += 1;
    if (_head == _cells + _cell_space - 1) {
        _head = _cells;
    } else {
        _head += 1;
    }
}

void Buffer::tailForward()
{
    _tail->setDataSize(0);
    if (_tail == _cells + _cell_space - 1) {
        _tail = _cells;
    } else {
        _tail += 1;
    }
}

unsigned int Buffer::headWholeSize() const
{
    return _head->wholeSize();
}

unsigned int Buffer::headDataSize() const
{
    return _head->dataSize();
}

void Buffer::setHeadDataSize(unsigned int size)
{
    _head->setDataSize(size);
}

unsigned int Buffer::tailWholeSize() const
{
    return _tail->wholeSize();
}

unsigned int Buffer::tailDataSize() const
{
    return _tail->dataSize();
}

void Buffer::setTailDataSize(unsigned int size)
{
    _tail->setDataSize(size);
}

bool Buffer::isFull() const
{
    return _head - _tail == _cell_space - 1 ||
           _head - _tail == -1;
}

bool Buffer::isEmpty() const
{
    return _head == _tail;
}

Buffer::Cell *Buffer::tailCell()
{
    return _tail;
}

const Buffer::Cell *Buffer::tailCell() const
{
    return _tail;
}

Buffer::Cell *Buffer::headCell()
{
    return _head;
}

const Buffer::Cell *Buffer::headCell() const
{
    return _head;
}

QDataStream &operator<<(QDataStream &stream, const Buffer &buffer)
{
    auto p = buffer._tail;
    while(p != buffer._head) {
        stream << *p;
        if (p == buffer._cells + buffer.space() - 1) {
            p = buffer._cells;
        } else {
            p += 1;
        }
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Buffer &buffer)
{
    stream >> *buffer._head;
    buffer.headForward();
    return stream;
}


Buffer::Cell::Cell() : _status(Status::UnInitialized), _cell(nullptr),
                       _whole_size(0), _data_size(0), _index(0) {}

Buffer::Cell::~Cell()
{
    if (_status != Status::UnInitialized) {
        delete[] _cell;
    }
}

void Buffer::Cell::initialize(unsigned int size)
{
    Q_ASSERT(size > 0);
    if (_status == Status::UnInitialized) {
        _cell = new VCI_CAN_OBJ[size];
//        _delay = delay;
        _status = Status::Initialized;
        _whole_size = size;
        for (auto i = 0U; i < size; ++i) {
            memset(_cell[i].Data, 0, sizeof(_cell[i].Data));
        }
    }
}

VCI_CAN_OBJ *Buffer::Cell::operator[](int index)
{
    Q_ASSERT(index >= 0);
    return _cell + index;
}

const VCI_CAN_OBJ *Buffer::Cell::operator[](int index) const
{
    Q_ASSERT(index >= 0);
    return _cell + index;
}

VCI_CAN_OBJ *Buffer::Cell::at(int index)
{
    Q_ASSERT(index >= 0);
    return _cell + index;
}

const VCI_CAN_OBJ *Buffer::Cell::at(int index) const
{
    Q_ASSERT(index >= 0);
    return _cell + index;
}

VCI_CAN_OBJ *Buffer::Cell::cell()
{
    return _cell;
}

const VCI_CAN_OBJ *Buffer::Cell::cell() const
{
    return _cell;
}

unsigned int Buffer::Cell::wholeSize() const
{
    return _whole_size;
}

unsigned int Buffer::Cell::dataSize() const
{
    return _data_size;
}

void Buffer::Cell::setDataSize(unsigned int size)
{
    Q_ASSERT(size <= _whole_size);
    _data_size = size;
}

//inline void Buffer::Cell::setDelay(const int delay) {
//    _delay = delay;
//}

//void Buffer::Cell::clear() {
//    _status = Status::UnUsed;
//}

//inline int Buffer::Cell::delay() const {
//    return _delay;
//}

QStringList Buffer::Cell::str() const
{
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
        case SendType::SelfSendRecieve:
            send_type = 2;
            break;
        case SendType::SelfSendRecieveOnce:
            send_type = 3;
            break;
    }
    for (auto i = 0U; i < dataSize(); ++i) {
        _cell[i].SendType = send_type;
    }
}

QDataStream &operator<<(QDataStream &stream, const Buffer::Cell &cell)
{
    stream << cell._index
           << cell._data_size
           << int(0)
           << int(0);
    for (unsigned int i = 0; i < cell._data_size; ++i) {
        stream << cell._cell[i];
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Buffer::Cell &cell)
{
    int buf;
    stream >> cell._index
           >> cell._data_size
           >> buf
           >> buf;
    for (unsigned int i = 0; i < cell._data_size; ++i) {
        stream >> cell._cell[i];
    }
    return stream;
}

void Buffer::Cell::setIndex(int index)
{
    _index = index;
}

int Buffer::Cell::index()
{
    return _index;
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

