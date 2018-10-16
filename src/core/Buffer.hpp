//
// Created by jiang.wenqiang on 2018/9/10.
//

#ifndef REFINE_BUFFER_HPP
#define REFINE_BUFFER_HPP

#include <QtCore/QList>
#include <QtCore/QDataStream>
#include "ControlCan.h"

/*!
 * @brief CAN的接收和发送缓冲区
 */

class Buffer
{
public:
    class Cell
    {
    public:
        enum SendType
        {
            Normal = 0,     //! \brief 正常发送
            Once = 1,       //! \brief 单次发送
            SelfSendReceive = 2,        //! \brief 自发自收
            SelfSendReceiveOnce = 3     //! \brief 单次自发自收
        };

    private:
        enum Status
        {
            UnInitialized,
            Initialized,
        };

    private:
        Status _status;
        VCI_CAN_OBJ *_objs;
        unsigned int _whole_size;
        unsigned int _data_size;
        int _index;

    public:
        Cell();
        ~Cell();
        void initialize(unsigned int size = 100);

        inline VCI_CAN_OBJ *operator[](int index) { return _objs + index; }

        inline const VCI_CAN_OBJ *operator[](int index) const
        {
            return _objs + index;
        }

        friend QDataStream &operator<<(QDataStream &stream, const Cell &cell);
        friend QDataStream &operator>>(QDataStream &stream, Cell &cell);

        inline VCI_CAN_OBJ *at(int index) { return _objs + index; }

        inline const VCI_CAN_OBJ *at(int index) const { return _objs + index; }

        inline VCI_CAN_OBJ *obj() { return _objs; }

        inline const VCI_CAN_OBJ *obj() const { return _objs; }

        inline unsigned int wholeSize() const { return _whole_size; }

        inline unsigned int dataSize() const { return _data_size; }

        inline void setDataSize(unsigned int size) { _data_size = size; }

        void setSendType(SendType type);

        QStringList str() const;

        inline void setIndex(int index) { _index = index; }

        inline int index() const { return _index; }

    };


    class Iter
    {
    private:
        Buffer *_buffer;
        int _pos;

    public:
        Iter(Buffer *buffer, int pos) : _buffer(buffer), _pos(pos) {}

        inline bool operator!=(const Iter &other) const
        {
            return _pos != other._pos;
        }

        inline Cell &operator*() const { return (*_buffer)[_pos]; }

        inline const Iter &operator++()
        {
            _pos += 1;
            _pos %= _buffer->_cell_space;  //!很重要
            return *this;
        }
    };

private:
    int _cell_space;
    int _index;
    Cell *_cells;
    int _head;
    int _tail;
    int _mark_head;
    int _mark_tail;
    bool _is_marked;

public:
    inline Buffer() : Buffer(50, 100) {}

    Buffer(const Buffer &buffer) = delete;
    explicit Buffer(int cell_space, unsigned int cell_size = 100);

    ~Buffer();

    Buffer &operator=(const Buffer &buffer) = delete;

    inline Cell &operator[](int index)
    {
        Q_ASSERT(index >= 0 && index < _cell_space);
        return *(_cells + index);
    }

    inline const Cell &operator[](int index) const
    {
        Q_ASSERT(index >= 0 && index < _cell_space);
        return *(_cells + index);
    }

    friend QDataStream &operator<<(QDataStream &stream, const Buffer &buffer);
    friend QDataStream &operator>>(QDataStream &stream, Buffer &buffer);

    inline int space() const { return _cell_space; }

    int size() const;
    void size(int &cell_n, int &obj_n) const;

    void headForward();
    void tailForward();

    inline unsigned int headWholeSize() const
    {
        return (_head + _cells)->wholeSize();
    }

    inline unsigned int headDataSize() const
    {
        return (_head + _cells)->dataSize();
    }

    inline unsigned int tailWholeSize() const
    {
        return (_tail + _cells)->wholeSize();
    }

    inline unsigned int tailDataSize() const
    {
        return (_tail + _cells)->dataSize();
    }

    inline void setHeadDataSize(unsigned int size)
    {
        (_head + _cells)->setDataSize(size);
    }

    inline void setTailDataSize(unsigned int size = 0)
    {
        (_tail + _cells)->setDataSize(size);
    }

    inline bool isFull() const
    {
        return _head - _tail == _cell_space - 1 ||
               _head - _tail == -1;
    }

    inline bool isEmpty() const { return _head == _tail; }

    inline Cell &tail() { return _cells[_tail]; }

    inline Cell &head() { return _cells[_head]; }

    inline const Cell &tail() const { return _cells[_tail]; }

    inline const Cell &head() const { return _cells[_head]; }

    Iter begin();

    Iter end();

    inline void setMark()
    {
        _mark_head = _head;
        _mark_tail = _tail;
        _is_marked = true;
    }

    inline void closeMark()
    {
        _tail = _mark_head;
        _is_marked = false;
    }

    inline bool isMarked() const { return _is_marked; }

    inline int headMarked() const
    {
        if (_is_marked){
            return _mark_head;
        } else {
            return -1;
        }
    }

    inline int tailMarked() const
    {
        if (_is_marked){
            return _mark_tail;
        } else {
            return -1;
        }
    }

    void reset();
};

QDataStream &operator<<(QDataStream &stream, const Buffer &buffer);
QDataStream &operator>>(QDataStream &stream, Buffer &buffer);

QDataStream &operator<<(QDataStream &stream, const Buffer::Cell &cell);
QDataStream &operator>>(QDataStream &stream, Buffer::Cell &cell);

QDataStream &operator<<(QDataStream &stream, const VCI_CAN_OBJ &obj);
QDataStream &operator>>(QDataStream &stream, VCI_CAN_OBJ &obj);

#endif //REFINE_BUFFER_HPP
