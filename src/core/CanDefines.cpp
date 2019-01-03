//
// Created by jiang.wenqiang on 2018/12/18.
//

#include "CanDefines.hpp"

QDataStream &operator<<(QDataStream &stream, const CanObj &obj)
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

QDataStream &operator>>(QDataStream &stream, CanObj &obj)
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

void setCanObj(CanObj &obj, unsigned int id,
               Cd::SendType send_type, const QByteArray &data, int data_len)
{

    obj.ID = id;
    obj.SendType = send_type;
    obj.RemoteFlag = Cd::RemoteFlag_False;
    obj.ExternFlag = Cd::ExternFlag_False;
    obj.DataLen = (BYTE_T) data_len;
    memcpy(obj.Data, data.data(), (size_t) data_len);
}