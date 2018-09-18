//
// Created by jiang.wenqiang on 2018/9/10.
//

#ifndef REFINE_BUFFER_H
#define REFINE_BUFFER_H

#include <QtCore/QList>
#include <QtCore/QDataStream>
#include "ControlCan.h"

class Buffer
{
public:
    class Cell;

private:
    int _cell_space;
    int _index;
    Cell *_cells;
    Cell *_head;
    Cell *_tail;

public:
    Buffer();
    Buffer(const Buffer &buffer) = delete;
    explicit Buffer(int cell_space, unsigned int cell_size = 100);
    ~Buffer();
    Buffer &operator=(const Buffer &buffer) = delete;
    Cell *operator[](int index);
    const Cell *operator[](int index) const;
    friend QDataStream &operator<<(QDataStream &stream, const Buffer &buffer);
    friend QDataStream &operator>>(QDataStream &stream, Buffer &buffer);
    int space() const;
    int size() const;
    void size(int &cell_n, int &obj_n) const;
    const VCI_CAN_OBJ *head() const;
    const VCI_CAN_OBJ *tail() const;
    VCI_CAN_OBJ *head();
    VCI_CAN_OBJ *tail();
    void headForward();
    void tailForward();
    unsigned int headWholeSize() const;
    unsigned int headDataSize() const;
    unsigned int tailWholeSize() const;
    unsigned int tailDataSize() const;
    void setHeadDataSize(unsigned int size);
    void setTailDataSize(unsigned int size = 0);
    bool isFull() const;
    bool isEmpty() const;
    Cell *tailCell();
    Cell *headCell();
    const Cell *tailCell() const;
    const Cell *headCell() const;

};

QDataStream &operator<<(QDataStream &stream, const Buffer &buffer);
QDataStream &operator>>(QDataStream &stream, Buffer &buffer);


class Buffer::Cell
{
    friend class Buffer;

public:
    enum SendType
    {
        Normal = 0,     //! \brief 正常发送
        Once = 1,       //! \brief 单次发送
        SelfSendRecieve = 2,        //! \brief 自发自收
        SelfSendRecieveOnce = 3     //! \brief 单次自发自收
    };

private:
    enum Status
    {
        UnInitialized,
        Initialized,
    };

private:
    Status _status;
    VCI_CAN_OBJ *_cell;
    unsigned int _whole_size;
    unsigned int _data_size;
    int _index;
//    int _delay;

public:
    Cell();
    ~Cell();
    void initialize(unsigned int size = 100);
    VCI_CAN_OBJ *operator[](int index);
    const VCI_CAN_OBJ *operator[](int index) const;
    friend QDataStream &operator<<(QDataStream &stream, const Cell &cell);
    friend QDataStream &operator>>(QDataStream &stream, Cell &cell);
    VCI_CAN_OBJ *at(int index);
    const VCI_CAN_OBJ *at(int index) const;
    VCI_CAN_OBJ *cell();
    const VCI_CAN_OBJ *cell() const;
    unsigned int wholeSize() const;
    unsigned int dataSize() const;
    void setDataSize(unsigned int size);
    void setSendType(SendType type);
    QStringList str() const;
//    void setDelay(int delay = -1);
//    void clear();
//    int delay() const;
    void setIndex(int index);
    int index();

};


QDataStream &operator<<(QDataStream &stream, const Buffer::Cell &cell);
QDataStream &operator>>(QDataStream &stream, Buffer::Cell &cell);
QDataStream &operator<<(QDataStream &stream, const VCI_CAN_OBJ &obj);
QDataStream &operator>>(QDataStream &stream, VCI_CAN_OBJ &obj);

#endif //REFINE_BUFFER_H
