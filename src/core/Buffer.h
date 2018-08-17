//
// Created by jiang.wenqiang on 2018/8/3.
//

#ifndef REFINE_BUFFER_H
#define REFINE_BUFFER_H

#include <QtCore/QList>
#include "ControlCan.h"

class Buffer {
public:
    class Cell;

    typedef Cell *BufferType;

private:
    int _size;
    int _index;
    BufferType _cells;
    BufferType _head;
    BufferType _tail;


public:
    Buffer() = delete;

    Buffer(const Buffer &buffer) = delete;

    explicit Buffer(int buffer_len = 50, unsigned long cell_size = 100);

    ~Buffer();

    Buffer &operator=(const Buffer &buffer) = delete;

    BufferType operator[](int index);

    int size() const;

    PVCI_CAN_OBJ head() const;

    PVCI_CAN_OBJ tail() const;

    void headForward();

    void tailForward();

    unsigned long headWholeSize() const;

    unsigned long headDataSize() const;

    void setHeadDataSize(unsigned long size);

    unsigned long tailWholeSize() const;

    unsigned long tailDataSize() const;

    void setTailDataSize(unsigned long size = 0);

    bool isFull() const;

    bool isEmpty() const;

    BufferType tailCell() const;

    BufferType headCell() const;
};


class Buffer::Cell {
    friend class Buffer;

private:
    enum class Status {
        UnInitialized,
        Initialized,
    };

private:
    Status _status;
    PVCI_CAN_OBJ _cell;
    unsigned long _whole_size;
    unsigned long _data_size;
//    int _delay;

public:
    Cell();

    ~Cell();

    void initialize(unsigned long size = 100);

    PVCI_CAN_OBJ operator[](int index) const;

    PVCI_CAN_OBJ cell() const;

    unsigned long wholeSize() const;

    unsigned long dataSize() const;

    void setDataSize(unsigned long size);

//    void setDelay(int delay = -1);
//    void clear();
//    int delay() const;
    QStringList str() const;

};


#endif //REFINE_BUFFER_H

