//
// Created by jiang.wenqiang on 2018/11/8.
//

#ifndef REFINE_FRAMEPOOL_HPP
#define REFINE_FRAMEPOOL_HPP

#include <QtCore/QVector>
#include "ControlCan.h"
#include "Message.hpp"

class Frame : public Message
{
public:
    class Cell
    {
    private:
        int _index;
        VCI_CAN_OBJ _data;
    public:
        Cell();

        Cell(int index, VCI_CAN_OBJ &&obj);

        // getter
        inline int index() const { return _index; }

        inline unsigned int id() const { return _data.ID; }

        inline unsigned int timeStamp() const { return _data.TimeStamp; }

        inline unsigned char timeFlag() const { return _data.TimeFlag; }

        inline unsigned char sendType() const { return _data.SendType; }

        inline unsigned char remoteFlag() const { return _data.RemoteFlag; }

        inline unsigned char externFlag() const { return _data.ExternFlag; }

        inline unsigned char dataLen() const { return _data.DataLen; }

        inline const unsigned char *data() const { return _data.Data; }

        inline const unsigned char *reserved() const { return _data.Reserved; }

        inline VCI_CAN_OBJ &frame() { return _data; }

        //str getter
        inline QString indexStr() const { return QString::number(_index); }

        inline QString idStr() const
        {
            return QString("0x%1").arg(_data.ID, 3, 16, QChar('0'));
        }

        inline QString timeStampStr() const { return QString(_data.TimeStamp); }

        inline QString timeFlagStr() const { return QString(_data.TimeFlag); }

        inline QString sendTypeStr() const { return QString(_data.SendType); }

        inline QString remoteFlagStr() const
        {
            return QString(_data.RemoteFlag);
        }

        inline QString externFlagStr() const
        {
            return QString(_data.ExternFlag);
        }

        inline QString dataLenStr() const { return QString(_data.DataLen); }

        QString dataStr() const;

        QString reservedStr() const;

    };

private:
    QVector<Cell> _cells;
    bool _initialized;
public:
    explicit Frame(Message *message = nullptr);

    friend QDataStream &operator>>(QDataStream &stream, Frame &pool);

    inline bool isEmpty() const { return _initialized; }

    inline void clear()
    {
        _cells.clear();
        _initialized = false;
    }

};

QDataStream &operator>>(QDataStream &stream, Frame &pool);

#endif //REFINE_FRAMEPOOL_HPP
