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

Ref::Ref() : _header(new Header()), _stream(new QDataStream()) {
    initTable();
    _stream->setVersion(QDataStream::Qt_5_11);
    _stream->setByteOrder(QDataStream::ByteOrder::LittleEndian);
}

bool Ref::loadFileHeader() {
    _stream->device()->seek(0);
    _header->clear();
    _stream->readRawData(_header->magic(), HEADER_MAGIC_L);
    (*_stream) >> _header->crc32();
    (*_stream) >> _header->type();
    _stream->readRawData(_header->info(), HEADER_INFO_L);
    (*_stream) >> _header->time();
    _stream->readRawData(_header->reserved(), HEADER_RESV_L);
    if (!_header->check()) {
        qDebug("bad header");
        return false;
    }
    QByteArray byte_array;
    signed char byte;
    _stream->device()->seek(HEADER_MAGIC_L + HEADER_CRC32_L);
    while (!_stream->atEnd()) {
        (*_stream) >> byte;
        byte_array.append(byte);
    }
    unsigned int check = crc32(byte_array);
    if (check != _header->crc32()) {
        qDebug() << "failed";
        return false;
    }
    return true;
}

void Ref::dumpFileHeader() {
    _stream->device()->seek(0);
    _stream->writeRawData(_header->magic(), HEADER_MAGIC_L);
    //写string时最好用以上函数，不然长度每次都不一样
    (*_stream) << _header->crc32();
    (*_stream) << _header->type();
    _stream->writeRawData(_header->info(), HEADER_INFO_L);
    (*_stream) << _header->time();
    _stream->writeRawData(_header->reserved(), HEADER_RESV_L);
}

void Ref::dumpFileHeaderCrc32() {
    _stream->device()->seek(HEADER_MAGIC_L + HEADER_CRC32_L);
    QByteArray byte_array;
    signed char byte;
    byte_array.clear();
    while (!_stream->atEnd()) {
        (*_stream) >> byte;
        byte_array.append(byte);
    }
    unsigned int check = crc32(byte_array);
    _stream->device()->seek(HEADER_MAGIC_L);
    (*_stream) << check;
}

bool Ref::loadCurveConfig(QFile &file, Curve &curve) {
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen()) {
        return false;
    }
    _stream->setDevice(&file);

    if (!loadFileHeader()) {
        return false;
    }
    if (!convertRefToCurveConfig(curve)) {
        return false;
    }
    _stream->unsetDevice();
    file.close();
    return true;
}

bool Ref::dumpCurveConfig(QFile &file, const Curve &curve) {
    file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    if (!file.isOpen()) {
        return false;
    }
    _stream->setDevice(&file);
    _header->clear();
    _header->setMagic();
    _header->setTime(QDateTime::currentDateTime().toTime_t());
    _header->setType(Header::Type::CurveConfig);
    char user[HEADER_INFO_L];
    char computer[HEADER_INFO_L];
    unsigned long user_l = HEADER_INFO_L;
    unsigned long computer_l = HEADER_INFO_L;
    GetUserNameA(user, &user_l);
    GetComputerNameA(computer, &computer_l);
    QString info = QString(user) + "@" + QString(computer);
    _header->setInfo(info);

    dumpFileHeader();

    file.seek(HEADER_L);
    convertCurveConfigToRef(curve);

    dumpFileHeaderCrc32();

    _stream->unsetDevice();
    file.close();

    return true;
}

#define CURVE_NAME_L 32
#define CURVE_UNIT_L 16
#define CURVE_REMARK_L 64

/*    unsigned short _index;
    bool _display;
    QString _name;
    Type _type;
    QString _unit;
    unsigned short _width;
    unsigned long _color;
    unsigned long _can_id;
    bool _zero_byte_existed;
    unsigned short _zero_byte;
    bool _high_byte_existed;
    unsigned short _high_byte;
    unsigned short _high_byte_range[2];
    unsigned short _low_byte;
    unsigned short _low_byte_range[2];
    Sample _sample_type;
    unsigned long _sample;
    long _range_in[2];
    long _range_out[2];
    Logic _logic_map;
    QString _remark;
    以上数据需要转换成可以更好的存储的数据，通过该函数
    index - char 1(byte)(下同)
    display - char 1
    type - char 1
    width - char 1

    name - char[32] 32
    unit - char[16] 16

    color - unsigned long 4
    can - unsigned long 4

    zero_exist - char 1
    zero_byte - char 1
    char[2]

    high_exist - char 1
    high_byte - char 1
    high_range - char[2] 2

    char[1]
    low_byte - char 1
    low_range - char[2] 2

    sample - char 1
    char[1]
    sample_rate short 2

    range_in long[2] 8
    range_out long[2] 8

    remark char[64];
    */

void Ref::convertCurveConfigToRef(const Curve &curve) {
    _stream->device()->seek(HEADER_L);
    _stream->writeRawData("VC", 2);
    (*_stream) << (unsigned short) curve.size();

    for (int i = 0; i < curve.size(); ++i) {
        (*_stream) << (unsigned char) curve[i].index();
        (*_stream) << (unsigned char) curve[i].display();
        (*_stream) << (unsigned char) curve[i].type();
        (*_stream) << (unsigned char) curve[i].width();

        char name[CURVE_NAME_L];
        memcpy(name, curve[i].name().toStdString().c_str(), CURVE_NAME_L);
        bool end = false;
        for (auto &iter : name) {
            if (iter == 0) {
                end = true;
            }
            if (end) {
                iter = 0;
            }
        }
        _stream->writeRawData(name, CURVE_NAME_L);

        char unit[CURVE_UNIT_L];
        memcpy(unit, curve[i].unit().toStdString().c_str(), CURVE_UNIT_L);
        end = false;
        for (auto &iter : name) {
            if (iter == 0) {
                end = true;
            }
            if (end) {
                iter = 0;
            }
        }
        _stream->writeRawData(unit, CURVE_UNIT_L);

        (*_stream) << (unsigned int) curve[i].color();
        (*_stream) << (unsigned int) curve[i].canId();

        (*_stream) << (unsigned char) curve[i].zeroByteExisted();
        (*_stream) << (unsigned char) curve[i].zeroByte();
        (*_stream) << (unsigned char) 0 << (unsigned char) 0;

        (*_stream) << (unsigned char) curve[i].highByteExisted();
        (*_stream) << (unsigned char) curve[i].highByte();
        (*_stream) << (unsigned char) curve[i].highByteRange()[0];
        (*_stream) << (unsigned char) curve[i].highByteRange()[1];

        (*_stream) << (unsigned char) 0;
        (*_stream) << (unsigned char) curve[i].lowByte();
        (*_stream) << (unsigned char) curve[i].lowByteRange()[0];
        (*_stream) << (unsigned char) curve[i].lowByteRange()[1];

        (*_stream) << (unsigned char) curve[i].sampleType();
        (*_stream) << (unsigned char) 0;
        (*_stream) << (unsigned short) curve[i].sample();

        (*_stream) << (int) curve[i].rangeIn()[0];
        (*_stream) << (int) curve[i].rangeIn()[1];

        (*_stream) << (int) curve[i].rangeOut()[0];
        (*_stream) << (int) curve[i].rangeOut()[1];

        char remark[64];
        memcpy(remark, curve[i].remark().toStdString().c_str(), 64);
        end = false;
        for (auto &iter : name) {
            if (iter == 0) {
                end = true;
            }
            if (end) {
                iter = 0;
            }
        }
        _stream->writeRawData(remark, 64);
    }
}

bool Ref::convertRefToCurveConfig(Curve &curve) {
    _stream->device()->seek(HEADER_L);
    char vc[2];
    _stream->readRawData(vc, 2);
    if (!(vc[0] == 'V' && vc[1] == 'C')) {
        return false;
    }
    unsigned short size;
    (*_stream) >> size;
    unsigned char char_buf;
    unsigned char char_buf1;
    unsigned int uint_buf;
    int int_buf;
    int int_buf1;
    unsigned short short_buf;
    for (int i = 0; i < size; ++i) {
        curve.appendRow();
        (*_stream) >> char_buf;
        curve[i].setIndexByVal(char_buf);
        (*_stream) >> char_buf;
        curve[i].setDisplayByVal(char_buf);
        (*_stream) >> char_buf;
        curve[i].setTypeByVal((Curve::Cell::Type) char_buf);
        (*_stream) >> char_buf;
        curve[i].setWidthByVal(char_buf);

        char name[CURVE_NAME_L];
        _stream->readRawData(name, CURVE_NAME_L);
        curve[i].setNameByVal(QString::fromUtf8(name));

        char unit[CURVE_UNIT_L];
        _stream->readRawData(unit, CURVE_UNIT_L);
        curve[i].setNameByVal(QString::fromUtf8(unit));

        (*_stream) >> uint_buf;
        curve[i].setColorByVal(uint_buf);
        (*_stream) >> uint_buf;
        curve[i].setCanIdByVal(uint_buf);

        (*_stream) >> char_buf;
        curve[i].setZeroByteExistedByVal(char_buf);
        (*_stream) >> char_buf;
        curve[i].setZeroByteByVal(char_buf);
        (*_stream) >> char_buf;
        (*_stream) >> char_buf;

        (*_stream) >> char_buf;
        curve[i].setHighByteExistedByVal(char_buf);
        (*_stream) >> char_buf;
        curve[i].setHighByteByVal(char_buf);
        (*_stream) >> char_buf;
        (*_stream) >> char_buf1;
        curve[i].setHighByteRangeByVal(char_buf, char_buf1);

        (*_stream) >> char_buf;
        (*_stream) >> char_buf;
        curve[i].setLowByteByVal(char_buf);
        (*_stream) >> char_buf;
        (*_stream) >> char_buf1;
        curve[i].setLowByteRangeByVal(char_buf, char_buf1);

        (*_stream) >> char_buf;
        curve[i].setSampleTypeByVal((Curve::Cell::Sample) char_buf);
        (*_stream) >> char_buf;
        (*_stream) >> short_buf;
        curve[i].setSampleByVal(short_buf);

        (*_stream) >> int_buf;
        (*_stream) >> int_buf1;
        curve[i].setRangeInByVal(int_buf, int_buf1);

        (*_stream) >> int_buf;
        (*_stream) >> int_buf1;
        curve[i].setRangeOutByVal(int_buf, int_buf1);

        char remark[CURVE_REMARK_L];
        _stream->readRawData(remark, CURVE_REMARK_L);
        curve[i].setNameByVal(QString::fromUtf8(remark));
    }
    return true;
}

bool Ref::loadRawData(QFile &file, Tribe &tribe) {
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen()) {
        return false;
    }
    QDataStream stream;
    _stream->setDevice(&file);
    _stream->setVersion(QDataStream::Qt_5_11);
    _stream->setByteOrder(QDataStream::ByteOrder::LittleEndian);
    Header header;

    if (!loadFileHeader()) {
        return false;
    }
    if (!convertRefToRawData(tribe)) {
        return false;
    }

    _stream->unsetDevice();
    file.close();
    return true;
}

bool Ref::pickRawData(QFile &file, Tribe &tribe, const QVector<int> &index) {
    return false;
}

bool Ref::beginDumpRawData(QFile &file, const Kebab &kebab) {
    file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    if (!file.isOpen()) {
        return false;
    }
    _stream->setDevice(&file);
    Header header(QDateTime::currentDateTime().toTime_t(),
                  Header::Type::RawData);
    char user[HEADER_INFO_L];
    char computer[HEADER_INFO_L];
    unsigned long user_l = HEADER_INFO_L;
    unsigned long computer_l = HEADER_INFO_L;
    GetUserNameA(user, &user_l);
    GetComputerNameA(computer, &computer_l);
    QString info = QString(user) + "@" + QString(computer);
    header.setInfo(info);
    dumpFileHeader();

    file.seek(HEADER_L);
    return true;
}

bool Ref::dumpRawData(const Kebab &kebab) {
    return false;
}

bool Ref::finishDumpRawData(const Kebab &kebab) {
    dumpFileHeaderCrc32();

}

void Ref::convertRawDataToRef(const Kebab &in) {

}

bool Ref::convertRefToRawData(Tribe &out) {
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

Ref::Header::Header() {
    clear();
}

Ref::Header::Header(const unsigned int time, const Ref::Header::Type type) {
    setMagic();
    _crc_sum = 0;
    memset(_info, '\0', sizeof(_info));
    _time = time;
    _type = (unsigned int) type;
    memset(_reserved, '\0', sizeof(_reserved));
    _reserved[HEADER_RESV_L - 1] = (char) 0xFF;
}

void Ref::Header::setMagic(const char *magic) {
    memcpy(_magic, magic, sizeof(_magic));
}

void Ref::Header::setMagic() {
    _magic[0] = (char) 0x89;
    _magic[1] = 'R';
    _magic[2] = 'E';
    _magic[3] = 'F';
    _magic[4] = Version::major;
    _magic[5] = Version::micro;
    _magic[6] = Version::minor;
    _magic[7] = (unsigned char) Version::identifier;
}

void Ref::Header::setType(const Ref::Header::Type type) {
    _type = (unsigned int) type;
}

void Ref::Header::setCrc32(const unsigned int crc32) {
    _crc_sum = crc32;
}

void Ref::Header::setInfo(const QString &info) {
    if (info.size() < HEADER_INFO_L) {
        memcpy(_info, info.toStdString().c_str(), info.size() * sizeof(char));
    } else {
        memcpy(_info, info.toStdString().c_str(), sizeof(_info));
    }
}

void Ref::Header::setInfo(const char *info) {
    memcpy(_info, info, sizeof(_info));
    bool end = false;
    for (auto &iter : _info) {
        if (iter == 0) {
            end = true;
        }
        if (end) {
            iter = 0;
        }
    }
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

bool Ref::Header::check() const {
    return _magic[0] == char(0x89) && _magic[1] == 'R' && _magic[2] == 'E' &&
           _magic[3] == 'F' && (_type == 0x00 || _type == 0x01 ||
                                _type == 0x02 || _type == 0x04 ||
                                _type == 0x08 || _type == 0x10);
}

void Ref::Header::clear() {
    memset(_magic, 0, sizeof(_magic));
    _crc_sum = 0;
    _type = 0;
    memset(_info, 0, sizeof(_info));
    _time = 0;
    memset(_reserved, 0, sizeof(_reserved));
    _reserved[HEADER_RESV_L - 1] = (char) 0xFF;
}
