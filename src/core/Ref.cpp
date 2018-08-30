//
// Created by jiang.wenqiang on 2018/8/15.
//

#include <QtCore/QDateTime>
#include "Ref.h"
#include "Version.h"


bool Ref::loadCurveConfig(QFile &file, Curve &curve) {
    return false;
}

bool Ref::dumpCurveConfig(QFile &file, const Curve &curve) {
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if (!file.isOpen()) {
        return false;
    }
    QDataStream stream(&file);
    Header header(QDateTime::currentDateTime().toTime_t(),
                  Header::Type::CurveConfig);
    char user[48];
    char computer[48];
    unsigned long user_l = 48;
    unsigned long computer_l = 48;
    GetUserNameA(user, &user_l);
    GetComputerNameA(computer, &computer_l);
    Q_ASSERT(user_l + computer_l < 48);
    QString info = QString(user) + "@" + QString(computer);
    header.setInfo(info);
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

Ref::Header::Header() : Header(0, Header::Type::None) {}

Ref::Header::Header(const unsigned int time, const Ref::Header::Type type) {
    _magic[0] = 89;
    _magic[1] = 'R';
    _magic[2] = 'E';
    _magic[3] = 'F';
    _magic[4] = Version::major;
    _magic[5] = Version::minor;
    _magic[6] = Version::micro;
    _magic[7] = (unsigned char) Version::identifier;
    _crc32 = 0;
    _info[0] = '\0';
    _time = time;
    _type = (unsigned long) type;
    _reserved[0] = 0;
}

void Ref::Header::setType(const Ref::Header::Type type) {
    _type = (unsigned long)type;
}

void Ref::Header::setCrc32(const long crc32) {
    _crc32 = crc32;
}

void Ref::Header::setInfo(const QString &info) {
    if (info.length() < 48) {
        memcpy(_info, info.toStdString().c_str(), 48* sizeof(char));
    } else {
        memcpy(_info, info.toStdString().c_str(), 47* sizeof(char));
        _info[47] = '0';
    }
}

void Ref::Header::setInfo(const char *info, const int len) {
    if (len <= 32) {
        memcpy(_info, info, 32* sizeof(char));
    } else {
        memcpy(_info, info, 31* sizeof(char));
        _info[31] = '0';
    }
}

void Ref::Header::setTime(const unsigned int time) {
    _time = time;
}

Ref::Header::Type Ref::Header::type() const {
    return (Type)_type;
}

long Ref::Header::crc32() const {
    return _crc32;
}

const char *Ref::Header::info() const {
    return _info;
}

unsigned int Ref::Header::time() const {
    return _time;
}
