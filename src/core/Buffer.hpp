/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/10
 * @brief 帧缓冲区
 * @details 采集时缓存报文的帧缓冲区
 ******************************************************************************/

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

        inline bool isEmpty() const { return _data_size == 0; }

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
        explicit Iter(Buffer *buffer = nullptr, int pos = 0) :
                _buffer(buffer), _pos(pos) {}

        inline void setParams(Buffer *buffer, int pos)
        {
            _buffer = buffer;
            _pos = pos;
        }

        inline bool operator!=(const Iter &other) const
        {
            return _pos != other._pos;
        }

        inline Cell &operator*() const { return *(_buffer->_cells + _pos); }

        inline const Iter &operator++()
        {
            _pos += 1;
            _pos %= _buffer->_cell_space;  //!很重要
            return *this;
        }

        inline const Iter &operator--()
        {
            _pos -= 1;
            _pos += _buffer->_cell_space;
            _pos %= _buffer->_cell_space;  //!很重要
            return *this;
        }
    };

private:
    int _cell_space;
    int _index;
    Cell *_cells;
    int _head;
    bool _empty;

public:
    inline Buffer() : Buffer(50, 100) {}

    Buffer(const Buffer &buffer) = delete;
    explicit Buffer(int cell_space, unsigned int cell_size = 100);

    ~Buffer();

    Buffer &operator=(const Buffer &buffer) = delete;

//    friend QDataStream &operator<<(QDataStream &stream, const Buffer &buffer);
    void dump(QDataStream &stream, Iter tail, Iter head);

    Cell &headCell() { return _cells[_head]; }

    inline int space() const { return _cell_space; }

    void move();

    inline Iter head() { return Iter(this, _head); }

    inline Iter last() { return --Iter(this, _head); }

    void size(Iter tail, Iter head, unsigned int &packs, unsigned int &frames);

    void reset();

    inline bool isEmpty() const { return _empty; };
};

//QDataStream &operator<<(QDataStream &stream, const Buffer &buffer);
QDataStream &operator>>(QDataStream &stream, Buffer &buffer);

QDataStream &operator<<(QDataStream &stream, const Buffer::Cell &cell);
QDataStream &operator>>(QDataStream &stream, Buffer::Cell &cell);

QDataStream &operator<<(QDataStream &stream, const VCI_CAN_OBJ &obj);
QDataStream &operator>>(QDataStream &stream, VCI_CAN_OBJ &obj);

#endif //REFINE_BUFFER_HPP
