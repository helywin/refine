//
// Created by jiang.wenqiang on 2018/8/15.
//

#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QDateTime>
#include <QtCore/QTimeZone>
#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <windows.h>
#include "Ref.h"
#include "Kebab.h"
#include "Tribe.h"
#include "Version.h"

Ref::Ref() {
    initTable();
}

bool Ref::loadCurveConfig(QFile &file, Curve &curve) {
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen()) {
        return false;
    }
    QDataStream stream;
    stream.setDevice(&file);
    stream.setVersion(QDataStream::Qt_5_11);
    stream.setByteOrder(QDataStream::ByteOrder::LittleEndian);
    Header header;

    file.seek(0);
    stream.readRawData(header.magic(), MAGIC_LEN);
    stream >> header.crc32();
    stream >> header.type();
    stream.readRawData(header.info(), INFO_LEN);
    stream >> header.time();
    stream.readRawData(header.reserved(), RESERVED_LEN);

    QByteArray byte_array;
    signed char byte;
    file.seek(MAGIC_LEN + CRC32_LEN);
    while (!stream.atEnd()) {
        stream >> byte;
        byte_array.append(byte);
    }
    unsigned int check = crc32(byte_array);
    if (check != header.crc32()) {
        qDebug() << "failed";
        return false;
    }
    return true;
}

bool Ref::dumpCurveConfig(QFile &file, const Curve &curve) {
    file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    if (!file.isOpen()) {
        return false;
    }
    Header header(QDateTime::currentDateTime().toTime_t(),
                  Header::Type::CurveConfig);
    char user[INFO_LEN];
    char computer[INFO_LEN];
    unsigned long user_l = INFO_LEN;
    unsigned long computer_l = INFO_LEN;
    GetUserNameA(user, &user_l);
    GetComputerNameA(computer, &computer_l);
    QString info = QString(user) + "@" + QString(computer);
    header.setInfo(info);
    const Header &header_c = header;
    QDataStream stream(&file);
    file.seek(0);
    stream.setVersion(QDataStream::Qt_5_11);
    //默认是BigEndian，坑爹
    stream.setByteOrder(QDataStream::ByteOrder::LittleEndian);
    stream.writeRawData(header_c.magic(), MAGIC_LEN);
    //写string时最好用以上函数，不然长度每次都不一样
    stream << header_c.crc32();
    stream << header_c.type();
    stream.writeRawData(header_c.info(), INFO_LEN);
    stream << header.time();
    stream.writeRawData(header_c.reserved(), RESERVED_LEN);

    //其他数据读写
    const char *b = "hello world!";
    stream.writeRawData(b, strlen(b));


    file.seek(MAGIC_LEN + CRC32_LEN);
    QByteArray byte_array;
    signed char byte;
    byte_array.clear();
    while (!stream.atEnd()) {
        stream >> byte;
        byte_array.append(byte);
    }
    unsigned int check = crc32(byte_array);
    file.seek(MAGIC_LEN);
    stream << check;
    file.close();
    return true;
}

bool Ref::loadRawData(QFile &file, Tribe &tribe) {
    return false;
}

bool Ref::dumpRawData(QFile &file, const Kebab &kebab) {
    return false;
}

bool Ref::loadModeConfig(QFile &file) {
    return false;
}

bool Ref::dumpModeConfig(QFile &file) {
    return false;
}

bool Ref::loadProData(QFile &file, Tribe &tribe) {
    return false;
}

bool Ref::dumpProData(QFile &file, const Tribe &tribe) {
    return false;
}

bool Ref::loadResult(QFile &file) {
    return false;
}

bool Ref::dumpResult(QFile &file) {
    return false;
}

unsigned int Ref::crc32(const QByteArray &array) const {
    unsigned int ret = 0xFFFFFFFF;
    for (const auto &byte : array) {
        ret = _crc32[((ret & 0xFFU) ^ (unsigned char) byte)] ^ (ret >> 8U);
    }
    ret = ~ret;
    return ret;
}

unsigned int Ref::crc32(const char *array, int len) const {
    unsigned int ret = 0xFFFFFFFF;
    for (int i = 0; i < len; ++i) {
        ret = _crc32[((ret & 0xFFU) ^ (unsigned char) array[i])] ^ (ret >> 8U);
    }
    ret = ~ret;
    return ret;
}

void Ref::initTable() {
    for (unsigned int i = 0; i < 256; ++i) {
        unsigned int crc = i;
        for (unsigned int j = 0; j < 8; ++j) {
            if (crc & 1U) {
                crc = (crc >> 1U) ^ 0xEDB88320U;
            } else {
                crc >>= 1;
            }
        }
        _crc32[i] = crc;
    }
}

Ref::Header::Header() : Header(0, Header::Type::None) {}

Ref::Header::Header(const unsigned int time, const Ref::Header::Type type) {
    _magic[0] = (char) 0x89;
    _magic[1] = 'R';
    _magic[2] = 'E';
    _magic[3] = 'F';
    _magic[4] = Version::major;
    _magic[5] = Version::micro;
    _magic[6] = Version::minor;
    _magic[7] = (unsigned char) Version::identifier;
    _crc_sum = 0;
    memset(_info, '\0', sizeof(_info));
    _time = time;
    _type = (unsigned int) type;
    memset(_reserved, '\0', sizeof(_reserved));
    _reserved[RESERVED_LEN - 1] = (char) 0xFF;
}

void Ref::Header::setMagic(const char *magic) {
    memcpy(_magic, magic, sizeof(_magic));
}

void Ref::Header::setType(const Ref::Header::Type type) {
    _type = (unsigned int) type;
}

void Ref::Header::setCrc32(const unsigned int crc32) {
    _crc_sum = crc32;
}

void Ref::Header::setInfo(const QString &info) {
    if (info.length() < INFO_LEN) {
        memcpy(_info, info.toStdString().c_str(), info.length() * sizeof(char));
    } else {
        memcpy(_info, info.toStdString().c_str(),
               (INFO_LEN - 1) * sizeof(char));
//        _info[47] = '\0';
    }
}

void Ref::Header::setInfo(const char *info) {
    memcpy(_info, info, sizeof(_info));
}

void Ref::Header::setTime(const unsigned int time) {
    _time = time;
}

void Ref::Header::setReserved(const char *reserved) {
    memcpy(_reserved, reserved, sizeof(_reserved));
}

const char *Ref::Header::magic() const {
    return _magic;
}

unsigned int Ref::Header::type() const {
    return _type;
}

unsigned int Ref::Header::crc32() const {
    return _crc_sum;
}

const char *Ref::Header::info() const {
    return _info;
}

unsigned int Ref::Header::time() const {
    return _time;
}

const char *Ref::Header::reserved() const {
    return _reserved;
}

QStringList Ref::Header::str() const {
    QStringList list;
    QString ver;
    switch ((unsigned char) _magic[7]) {
        case 0x00:
            ver = QString("Alpha");
            break;
        case 0x01:
            ver = QString("Beta");
            break;
        case 0x02:
            ver = QString("Gamma");
            break;
        case 0x03:
            ver = QString("Release");
            break;
        case 0xFF:
            ver = QString("Demo");
            break;
        default:
            ver = QString("Unknown");
            break;
    }
    list.append(QString("%1.%2.%3(%4)")
                        .arg(_magic[4])
                        .arg(_magic[5])
                        .arg(_magic[6])
                        .arg(ver));
    QString type;
    switch (_type) {
        case 0x01:
            type = QString("Curve Config");
            break;
        case 0x02:
            type = QString("Raw Data");
            break;
        case 0x04:
            type = QString("Mode Config");
            break;
        case 0x08:
            type = QString("Processed Data");
            break;
        case 0x10:
            type = QString("Result");
            break;
        default:
            type = QString("None");
            break;
    }
    list.append(qMove(type));
    list.append(_info);
    QString date = QDateTime::fromTime_t(_time).
            toString(QString("yyyy/MM/dd hh:mm:ss"));
    list.append(qMove(date));
    return qMove(list);
}

char *Ref::Header::magic() {
    return _magic;
}

unsigned int &Ref::Header::crc32() {
    return _crc_sum;
}

unsigned int &Ref::Header::type() {
    return _type;
}

char *Ref::Header::info() {
    return _info;
}

unsigned int &Ref::Header::time() {
    return _time;
}

char *Ref::Header::reserved() {
    return _reserved;
}
