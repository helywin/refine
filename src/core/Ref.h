//
// Created by jiang.wenqiang on 2018/8/15.
//

#ifndef REFINE_REF_H
#define REFINE_REF_H

#include <QtCore/QList>
#include <QtCore/QDataStream>
#include <windows.h>
#include "Curve.h"
#include "Kebab.h"
#include "Tribe.h"
#include "Modes.h"


class Ref {
public:
    class Header;

public:
    Ref() = default;

    bool loadCurveConfig(QFile &file, Curve &curve);

    bool dumpCurveConfig(QFile &file, const Curve &curve);

    bool loadRawData(QFile &file, Tribe &tribe);

    bool dumpRawData(QFile &file, const Kebab &kebab);

    bool loadModeConfig(QFile &file);

    bool dumpModeConfig(QFile &file);

    bool loadProData(QFile &file, Tribe &tribe);

    bool dumpProData(QFile &file, const Tribe &tribe);

    bool loadResult(QFile &file);

    bool dumpResult(QFile &file);

};

class Ref::Header {
public:
    enum class Type {
        None = 0x00,
        CurveConfig = 0x01,     //.cc
        RawData = 0x02,         //.rd
        ModeConfig = 0x04,      //.mc
        ProData = 0x08,         //.cd
        Result = 0x10           //.rs
    };
private:
    //! \brief 四字节对齐
    unsigned char _magic[8];                  // 8 byte
    unsigned long _type;                      // 4 byte
    long _crc32;                              // 4 byte
    char _info[48];                           // 32 byte
    unsigned int _time;                               // 4 byte
    unsigned char _reserved[128];             // 128 byte
public:
    Header();

    Header(unsigned int time, Type type);

    Header(const Header &header) = default;

    ~Header() = default;

    //getter and setter
    void setType(Type type);

    void setCrc32(long crc32);

    void setInfo(const QString &info);

    void setInfo(const char *info, int len);

    void setTime(unsigned int time);

    Type type() const;

    long crc32() const;

    const char *info() const;

    unsigned int time() const;
};


#endif //REFINE_REF_H
