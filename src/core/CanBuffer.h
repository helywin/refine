//
// Created by jiang.wenqiang on 2018/6/29.
//

#ifndef CORE_CANBUFFER_H
#define CORE_CANBUFFER_H

#include "ControlCan.h"
#include <QtCore/QMutex>
#include <QtCore/QStringList>

class CanBufferCell {
public:
<<<<<<< HEAD
    //! \brief 发送帧缓冲区
    PVCI_CAN_OBJ buffer;

    //! \brief 缓冲区大小
    unsigned long size;

    //! \brief 实际长度
    unsigned long l;
=======
    class Cell;

private:
    unsigned int length;

    Cell *buffer_list;

    unsigned int index;
>>>>>>> 568e96c... 准备重新开发结构

    //! \brief 计数器
    unsigned long index;

    int delay;

public:

    CanBufferCell() = default;

    void init(unsigned long size = 1, int delay = -1);

    ~CanBufferCell();

    void clear();

//    CanBufferCell(const CanBufferCell &cell);

//    CanBufferCell &operator=(const CanBufferCell &cell);

    PVCI_CAN_OBJ operator[](unsigned int index);

    void str(QStringList &list);

    static QString header();

};

<<<<<<< HEAD
=======
class CanBuffer::Cell {
    friend CanBuffer;
public:
    //! \brief 帧缓冲区
    PVCI_CAN_OBJ _buffer;

    //! \brief 缓冲区大小
    unsigned long _size;

    //! \brief 实际长度
    unsigned long _length;

    //! \brief 计数器
    unsigned long _index;

    int _delay;
>>>>>>> 568e96c... 准备重新开发结构

/**
 * @brief 接收CAN帧的数据结构
 * 循环队列
 */
class CanBuffer {
public:
    CanBufferCell *buffer_list;

    unsigned int length;

private:
    unsigned int index;

    CanBufferCell *head_point;

    CanBufferCell *tail_point;

public:
    CanBuffer() = delete;

    CanBuffer(unsigned int length, unsigned int size);

    ~CanBuffer();
//    void resize(unsigned int length, unsigned int size);

    CanBufferCell *out();

    CanBufferCell *push();

    CanBufferCell *head();

    CanBufferCell *tail();

    bool empty();

    bool full();


private:
    void inc();

    void dec();
};


#endif //CORE_CANBUFFER_H
