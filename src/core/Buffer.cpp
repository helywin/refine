//
// Created by jiang.wenqiang on 2018/9/10.
//

#include "Buffer.h"

Buffer::Buffer() : Buffer(50, 100) {}

Buffer::Buffer(int buffer_len, unsigned long cell_size) :
        _size(buffer_len), _index(0), _cells(new Cell[buffer_len]),
        _head(_cells), _tail(_cells)
{
    Q_ASSERT(buffer_len > 0);
    Q_ASSERT(cell_size > 0);
    for (int i = 0; i < _size; ++i) {
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

int Buffer::size() const
{
    return _size;
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
    if (_head == _cells + _size - 1) {
        _head = _cells;
    } else {
        _head += 1;
    }
}

void Buffer::tailForward()
{
    _tail->setDataSize(0);
    if (_tail == _cells + _size - 1) {
        _tail = _cells;
    } else {
        _tail += 1;
    }
}

unsigned long Buffer::headWholeSize() const
{
    return _head->wholeSize();
}

unsigned long Buffer::headDataSize() const
{
    return _head->dataSize();
}

void Buffer::setHeadDataSize(const unsigned long size)
{
    _head->setDataSize(size);
}

unsigned long Buffer::tailWholeSize() const
{
    return _tail->wholeSize();
}

unsigned long Buffer::tailDataSize() const
{
    return _tail->dataSize();
}

void Buffer::setTailDataSize(const unsigned long size)
{
    _tail->setDataSize(size);
}

bool Buffer::isFull() const
{
    return _head - _tail == _size - 1 ||
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

Buffer::Cell::Cell() : _status(Status::UnInitialized), _cell(nullptr),
                       _whole_size(0), _data_size(0) {}

Buffer::Cell::~Cell()
{
    if (_status != Status::UnInitialized) {
        delete[] _cell;
    }
}

void Buffer::Cell::initialize(unsigned long size)
{
    Q_ASSERT(size > 0);
    if (_status == Status::UnInitialized) {
        _cell = new VCI_CAN_OBJ[size];
//        _delay = delay;
        _status = Status::Initialized;
        _whole_size = size;
        for (auto i = 0U; i < size; ++i) {
            _cell[i].Data[0] = 0;
            _cell[i].Data[1] = 0;
            _cell[i].Data[2] = 0;
            _cell[i].Data[3] = 0;
            _cell[i].Data[4] = 0;
            _cell[i].Data[5] = 0;
            _cell[i].Data[6] = 0;
            _cell[i].Data[7] = 0;
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

unsigned long Buffer::Cell::wholeSize() const
{
    return _whole_size;
}

unsigned long Buffer::Cell::dataSize() const
{
    return _data_size;
}

void Buffer::Cell::setDataSize(const unsigned long size)
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

