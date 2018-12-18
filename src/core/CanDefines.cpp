//
// Created by jiang.wenqiang on 2018/12/18.
//

#include "CanDefines.hpp"

QDataStream &operator<<(QDataStream &stream, const VCI_CAN_OBJ &obj)
{
    stream << obj.ID
           << obj.TimeStamp
           << obj.TimeFlag
           << obj.SendType
           << obj.RemoteFlag
           << obj.ExternFlag
           << obj.DataLen;
    stream.writeRawData((const char *) obj.Data, 8);
    stream.writeRawData((const char *) obj.Reserved, 3);
    return stream;
}

QDataStream &operator>>(QDataStream &stream, VCI_CAN_OBJ &obj)
{
    stream >> obj.ID
           >> obj.TimeStamp
           >> obj.TimeFlag
           >> obj.SendType
           >> obj.RemoteFlag
           >> obj.ExternFlag
           >> obj.DataLen;
    stream.readRawData((char *) obj.Data, 8);
    stream.readRawData((char *) obj.Reserved, 3);
    return stream;
}

VCI_CAN_OBJ canObj(unsigned int id, Cd::SendType send_type, QByteArray &&data)
{
    VCI_CAN_OBJ obj;
    obj.ID = id;
    obj.SendType = send_type;
    obj.DataLen = 8;
    memcpy(obj.Data, data.data(), 8);
    return obj;
}