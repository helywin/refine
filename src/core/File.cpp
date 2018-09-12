//
// Created by jiang.wenqiang on 2018/9/12.
//

#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <windows.h>
#include "File.h"
#include "Version.h"

File::File()
        : _header(new File::Header()),
          _stream(new QDataStream())
{
    initTable();
    _stream->setVersion(QDataStream::Qt_5_11);
    _stream->setByteOrder(QDataStream::ByteOrder::LittleEndian);
}

File::~File()
{
    delete _header;
    delete _stream;
}

void File::initTable()
{
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

unsigned int File::crc32(const QByteArray &array) const
{
    unsigned int ret = 0xFFFFFFFF;
    for (const auto &byte : array) {
        ret = _crc32[((ret & 0xFFU) ^ static_cast<unsigned char> (byte))] ^
              (ret >> 8U);
    }
    ret = ~ret;
    return ret;
}

unsigned int File::crc32(const char *array, int len) const
{
    unsigned int ret = 0xFFFFFFFF;
    for (int i = 0; i < len; ++i) {
        ret = _crc32[((ret & 0xFFU) ^ static_cast<unsigned char> (array[i]))] ^
              (ret >> 8U);
    }
    ret = ~ret;
    return ret;
}

bool File::loadFileHeader()
{
    _stream->device()->seek(0);
    _header->clear();
    _stream->readRawData(_header->magic(), HEADER_MAGIC_L);
    _stream->readRawData(_header->version(), HEADER_VERSION_L);
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
    _stream->device()->seek(HEADER_MAGIC_L +
                            HEADER_VERSION_L +
                            HEADER_CRC32_L);
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

void File::dumpFileHeader()
{
    _stream->device()->seek(0);
    //写char[]时最好用以上函数，不然长度每次都不一样
    _stream->writeRawData(_header->magic(), HEADER_MAGIC_L);
    _stream->writeRawData(_header->version(), HEADER_VERSION_L);
    (*_stream) << _header->crc32();
    (*_stream) << _header->type();
    _stream->writeRawData(_header->info(), HEADER_INFO_L);
    (*_stream) << _header->time();
    _stream->writeRawData(_header->reserved(), HEADER_RESV_L);
}

void File::dumpFileHeaderCrc32()
{
    _stream->device()->seek(HEADER_MAGIC_L +
                            HEADER_VERSION_L +
                            HEADER_CRC32_L);
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

bool File::loadCurveConfig(QFile &file, Curve &curve)
{
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

bool File::dumpCurveConfig(QFile &file, const Curve &curve)
{
    file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    if (!file.isOpen()) {
        return false;
    }
    _stream->setDevice(&file);
    _header->clear();
    _header->setMagic();
    _header->setVersion();
    _header->setTime(QDateTime::currentDateTime().toTime_t());
    _header->setType(Header::FileType ::CurveConfig);
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

void File::convertCurveConfigToRef(const Curve &curve)
{

}

bool File::convertRefToCurveConfig(Curve &curve)
{
    return false;
}


File::Header::Header()
{
    clear();
}

File::Header::Header(unsigned int time, File::Header::FileType type)
{
    setMagic();
    setVersion();
    _crc_sum = 0;
    memset(_info, '\0', sizeof(_info));
    _time = time;
    _type = static_cast<unsigned int> (type);
    memset(_reserved, '\0', sizeof(_reserved));
    _reserved[HEADER_RESV_L - 1] = static_cast<char> (0xFF);
}

void File::Header::setMagic(const char *magic)
{
    memcpy(_magic, magic, sizeof(_magic));
}

void File::Header::setMagic()
{
    _magic[0] = static_cast<char>(0x89);
    _magic[1] = 'R';
    _magic[2] = 'E';
    _magic[3] = 'F';
    _magic[4] = 'I';
    _magic[5] = 'N';
    _magic[6] = 'E';
    _magic[7] = static_cast<char>(0x00);
}

void File::Header::setVersion(const char *version)
{
    memcpy(_version, version, sizeof(_version));
}

void File::Header::setVersion()
{
    _version[0] = Version::major();
    _version[1] = Version::micro();
    _version[2] = Version::minor();
    _version[3] = Version::build();
    _version[4] = Version::year();
    _version[5] = Version::month();
    _version[6] = Version::day();
    _version[7] = Version::identifier();
}

void File::Header::setCrc32(unsigned int crc32)
{
    _crc_sum = crc32;
}

void File::Header::setType(File::Header::FileType type)
{
    _type = type;
}

void File::Header::setInfo(const QString &info)
{
    if (info.size() < HEADER_INFO_L) {
        memcpy(_info, info.toStdString().c_str(), info.size() * sizeof(char));
    } else {
        memcpy(_info, info.toStdString().c_str(), sizeof(_info));
    }
}

void File::Header::setInfo(const char *info)
{
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

void File::Header::setTime(unsigned int time)
{
    _time = time;
}

void File::Header::setReserved(const char *reserved)
{
    memcpy(_reserved, reserved, sizeof(_reserved));
}

const char *File::Header::magic() const
{
    return _magic;
}

const char *File::Header::version() const
{
    return _version;
}

unsigned int File::Header::crc32() const
{
    return _crc_sum;
}

unsigned int File::Header::type() const
{
    return _type;
}

const char *File::Header::info() const
{
    return _info;
}

unsigned int File::Header::time() const
{
    return _time;
}

const char *File::Header::reserved() const
{
    return _reserved;
}

char *File::Header::magic()
{
    return _magic;
}

char *File::Header::version()
{
    return _version;
}

unsigned int &File::Header::crc32()
{
    return _crc_sum;
}

unsigned int &File::Header::type()
{
    return _type;
}

char *File::Header::info()
{
    return _info;
}

unsigned int &File::Header::time()
{
    return _time;
}

char *File::Header::reserved()
{
    return _reserved;
}

QStringList File::Header::str() const
{
    QStringList list;
    QString ver;
    switch (_version[7]) {
        case Version::None:
            ver = QString();
            break;
        case Version::Demo:
            ver = QString("Demo");
            break;
        case Version::Alpha:
            ver = QString("Alpha");
            break;
        case Version::Beta:
            ver = QString("Beta");
            break;
        case Version::Gamma:
            ver = QString("Gamma");
            break;
        case Version::Release:
            ver = QString("Release");
            break;
        default:
            ver = QString("Unknown");
            break;
    }
    list.append(QString("%1.%2.%3.%4_%5%6%7(%8)")
                        .arg(_version[0])
                        .arg(_version[1])
                        .arg(_version[2])
                        .arg(_version[3])
                        .arg(_version[4])
                        .arg(_version[5])
                        .arg(_version[6])
                        .arg(ver));
    QString type;
    switch (_type) {
        case File::Header::FileType::None :
            type = QString("None");
            break;
        case File::Header::FileType::CurveConfig :
            type = QString("Curve Config");
            break;
        case File::Header::FileType::FrameData :
            type = QString("Frame Data");
            break;
        case File::Header::FileType::RawData :
            type = QString("Raw Data");
            break;
        case File::Header::FileType::ModeConfig :
            type = QString("Mode Config");
            break;
        case File::Header::FileType::ProData :
            type = QString("Processed Data");
            break;
        case File::Header::FileType::Result :
            type = QString("Result");
            break;
        default:
            type = QString("Unknown");
            break;
    }
    list.append(qMove(type));
    list.append(_info);
    QString date = QDateTime::fromTime_t(_time).
            toString(QString("yyyy/MM/dd hh:mm:ss"));
    list.append(qMove(date));
    return qMove(list);
}

bool File::Header::check() const
{
    char header[] = "0REFINE0";
    header[0] = static_cast<char>(0x89);
    return static_cast<bool> (strcmp(_magic, header));
}

void File::Header::clear()
{
    memset(_magic, 0, sizeof(_magic));
    memset(_version, 0, sizeof(_version));
    _crc_sum = 0;
    _type = 0;
    memset(_info, 0, sizeof(_info));
    _time = 0;
    memset(_reserved, 0, sizeof(_reserved));
    _reserved[HEADER_RESV_L - 1] = (char) 0xFF;
}
