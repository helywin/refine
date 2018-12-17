//
// Created by jiang.wenqiang on 2018/11/8.
//

#include "Frame.hpp"
#include "Buffer.hpp"
#include "File.hpp"

Frame::Cell::Cell() :
        _index(0) {}

QString Frame::Cell::dataStr() const
{
    QStringList list;
    for (const auto &d : _data.Data) {
        list.append(QString("%1").arg(d, 2, 16, QChar('0')));
    }
    return list.join(QChar(' '));
}

QString Frame::Cell::reservedStr() const
{
    QStringList list;
    for (const auto &d : _data.Reserved) {
        list.append(QString("%1").arg(d, 2, 16, QChar('0')));
    }
    return list.join(QChar(' '));
}

Frame::Cell::Cell(int index, VCI_CAN_OBJ &&obj) :
        _index(index),
        _data(obj) {}


Frame::Frame(Message *message) :
        Message(message), _initialized(false) {}

QDataStream &operator>>(QDataStream &stream, Frame &pool)
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
        for (int j = 0; j < cell.dataSize(); ++j) {
            pool._cells.append(Frame::Cell(i, qMove(*cell[j])));
        }
    }
    return stream;
}
