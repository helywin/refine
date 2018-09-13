//
// Created by jiang.wenqiang on 2018/9/10.
//

#ifndef REFINE_BUFFER_H
#define REFINE_BUFFER_H

#include <QtCore/QList>
#include "ControlCan.h"

class Buffer
{
public:
    class Cell;

private:
    int _size;
    int _index;
    Cell *_cells;
    Cell *_head;
    Cell *_tail;

public:
    Buffer();
    Buffer(const Buffer &buffer) = delete;
    explicit Buffer(int buffer_len, unsigned long cell_size = 100);
    ~Buffer();
    Buffer &operator=(const Buffer &buffer) = delete;
    Cell *operator[](int index);
    const Cell *operator[](int index) const;
    int size() const;
    const VCI_CAN_OBJ *head() const;
    const VCI_CAN_OBJ *tail() const;
    VCI_CAN_OBJ *head();
    VCI_CAN_OBJ *tail();
    void headForward();
    void tailForward();
    unsigned long headWholeSize() const;
    unsigned long headDataSize() const;
    unsigned long tailWholeSize() const;
    unsigned long tailDataSize() const;
    void setHeadDataSize(unsigned long size);
    void setTailDataSize(unsigned long size = 0);
    bool isFull() const;
    bool isEmpty() const;
    Cell *tailCell();
    Cell *headCell();
    const Cell *tailCell() const;
    const Cell *headCell() const;
};


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
    unsigned long _whole_size;
    unsigned long _data_size;
//    int _delay;

public:
    Cell();
    ~Cell();
    void initialize(unsigned long size = 100);
    VCI_CAN_OBJ *operator[](int index);
    const VCI_CAN_OBJ *operator[](int index) const;
    VCI_CAN_OBJ *at(int index);
    const VCI_CAN_OBJ *at(int index) const;
    VCI_CAN_OBJ *cell();
    const VCI_CAN_OBJ *cell() const;
    unsigned long wholeSize() const;
    unsigned long dataSize() const;
    void setDataSize(unsigned long size);
    void setSendType(SendType type);
    QStringList str() const;
//    void setDelay(int delay = -1);
//    void clear();
//    int delay() const;


};

#endif //REFINE_BUFFER_H
