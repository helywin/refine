//
// Created by jiang.wenqiang on 2018/11/8.
//

#include "FramePool.hpp"
#include "Buffer.hpp"
#include "File.hpp"

FramePool::Cell::Cell() :
        _index(0) {}

QString FramePool::Cell::dataStr() const
{
    QStringList list;
    for (const auto &d : _data.Data) {
        list.append(QString("%1").arg(d, 2, 16, QChar('0')));
    }
    return list.join(QChar(' '));
}

QString FramePool::Cell::reservedStr() const
{
    QStringList list;
    for (const auto &d : _data.Reserved) {
        list.append(QString("%1").arg(d, 2, 16, QChar('0')));
    }
    return list.join(QChar(' '));
}

FramePool::Cell::Cell(int index, VCI_CAN_OBJ &&obj) :
        _index(index),
        _data(obj) {}


FramePool::FramePool() : _initialized(false) {}

QDataStream &operator>>(QDataStream &stream, FramePool &pool)
{
    unsigned int obj_num;
    unsigned int cell_num;
    int reserved;
    stream >> obj_num
           >> cell_num
           >> reserved
           >> reserved;
    for (int i = 0; i < cell_num; ++i) {
        Buffer::Cell cell;
        stream >> cell;
        for (int j = 0; j < cell.dataSize(); ++ j) {
            pool._cells.append(FramePool::Cell(i, qMove(*cell[j])));
        }
    }
    return stream;
}
