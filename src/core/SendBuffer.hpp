//
// Created by jiang.wenqiang on 2018/12/18.
//

#ifndef REFINE_SENDBUFFER_HPP
#define REFINE_SENDBUFFER_HPP


#include <QtCore/QVector>
#include "CanDefines.hpp"

class SendBuffer
{
public:

    /*!
     * @brief 普通迭代器
     */
    class Iter
    {
    private:
        SendBuffer *_buffer;
        int _pos;

    public:
        explicit Iter(SendBuffer *buffer = nullptr, int pos = 0) :
                _buffer(buffer), _pos(pos)
        {}

        inline void setParams(SendBuffer *buffer, int pos)
        {
            _buffer = buffer;
            _pos = pos;
        }

        inline bool operator!=(const Iter &other) const
        {
            return _pos != other._pos;
        }

        inline CanObj &operator*() const
        { return *(_buffer->_cells + _pos); }

        inline const Iter &operator++()
        {
            _pos += 1;
            _pos %= _buffer->_len;  //!很重要
            return *this;
        }

        inline const Iter &operator--()
        {
            _pos -= 1;
            _pos += _buffer->_len;
            _pos %= _buffer->_len;  //!很重要
            return *this;
        }
    };

    /*!
     * @brief const迭代器
     */
    class ConstIter
    {
    private:
        SendBuffer *_buffer;
        int _pos;

    public:
        explicit ConstIter(SendBuffer *buffer = nullptr, int pos = 0) :
                _buffer(buffer), _pos(pos)
        {}

        inline void setParams(SendBuffer *buffer, int pos)
        {
            _buffer = buffer;
            _pos = pos;
        }

        inline bool operator!=(const ConstIter &other) const
        {
            return _pos != other._pos;
        }

        inline const CanObj &operator*() const
        { return *(_buffer->_cells + _pos); }

        inline const ConstIter &operator++()
        {
            _pos += 1;
            _pos %= _buffer->_len;  //!很重要
            return *this;
        }

        inline const ConstIter &operator--()
        {
            _pos -= 1;
            _pos += _buffer->_len;
            _pos %= _buffer->_len;  //!很重要
            return *this;
        }
    };
private:
    CanObj *_cells;
    int _len;
    int _begin;
    int _end;
public:
    explicit SendBuffer(int len = 10000);
    SendBuffer(const SendBuffer &buffer) = delete;
    SendBuffer &operator=(const SendBuffer &buffer) = delete;
    ~SendBuffer();

    inline Iter begin()
    { return Iter(this, _begin); }

    inline Iter end()
    { return Iter(this, _end); }

    inline ConstIter constBegin()
    { return ConstIter(this, _begin); }

    inline ConstIter constEnd()
    { return ConstIter(this, _end); }

    inline bool isEmpty() const
    { return size() == 0; }

    inline bool isFull() const
    { return size() == _len - 1; }

    inline int size() const
    { return (_end - _begin + _len) % _len; }

    inline void reset()
    {
        _begin = 0;
        _end = 0;
    }

    inline void increase()
    {
        _end += 1;
        _end %= _len;
    }

    inline void decrease()
    {
        _begin += 1;
        _begin %= _len;
    }

    inline void operator++()
    { increase(); }

    inline void operator--()
    { decrease(); }

    inline void move(int num)
    {
        Q_ASSERT(num <= size());
        _begin += num;
        _begin %= _len;
        Q_ASSERT(_begin >= 0 && _begin < _len);
    }


};


#endif //REFINE_SENDBUFFER_HPP
