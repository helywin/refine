//
// Created by jiang.wenqiang on 2018/6/29.
//

#ifndef CORE_CANBUFFER_H
#define CORE_CANBUFFER_H

#include "ControlCan.h"
#include <QtCore/QMutex>
#include <QtCore/QStringList>

/**
 * @brief 接收CAN帧的数据结构
 * 循环队列
 */
class CanBuffer {
public:
    class Cell;

    Cell *buffer_list;

    unsigned int length;

private:
    unsigned int index;

    Cell *head_point;

    Cell *tail_point;

public:
    CanBuffer() = delete;

    CanBuffer(unsigned int length, unsigned int size);

    ~CanBuffer();
//    void resize(unsigned int length, unsigned int _size);

    Cell *out();

    Cell *push();

    Cell *head();

    Cell *tail();

    bool empty();

    bool full();


private:
    void inc();

    void dec();
};

class CanBuffer::Cell {
    friend CanBuffer;
private:
    //! \brief 帧缓冲区
    PVCI_CAN_OBJ _buffer;

    //! \brief 缓冲区大小
    unsigned long _size;

    //! \brief 实际长度
    unsigned long _length;

    //! \brief 计数器
    unsigned long _index;

    int _delay;

public:

    Cell() = default;

    void init(unsigned long size = 1, int delay = -1);

    ~Cell();

//    Cell(const Cell &cells);

//    Cell &operator=(const Cell &cells);

    PVCI_CAN_OBJ operator[](unsigned int index);

    void str(QStringList &list);

    static QString header();

    inline unsigned long length() const { return _length; };

    inline PVCI_CAN_OBJ buffer() { return _buffer; };

    inline void clear() { _length = 0; }

    inline void setLength(unsigned int length) { _length = length; }

    inline unsigned int size() const { return _size; }

    inline unsigned int delay() const { return _delay; }

};

#endif //CORE_CANBUFFER_H
