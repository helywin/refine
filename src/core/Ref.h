//
// Created by jiang.wenqiang on 2018/8/15.
//

#ifndef REFINE_REF_H
#define REFINE_REF_H

class QStringList;

class QString;

class QFile;

class QByteArray;

class Curve;

class Tribe;

class Modes;

class Kebab;

class Ref {
public:
    class Header;

private:
    unsigned int _crc32[256];

public:
    Ref();

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

    unsigned int crc32(const QByteArray &array) const;

    unsigned int crc32(const char *array, int len) const;

private:
    void initTable();

};

#define MAGIC_LEN 8
#define CRC32_LEN 4
#define TYPE_LEN 4
#define INFO_LEN 44
#define TIME_LEN 4
#define RESERVED_LEN 128
#define HEADER_LEN (MAGIC_LEN + INFO_LEN + RESERVED_LEN + \
                    TYPE_LEN + CRC32_LEN + TIME_LEN)

class Ref::Header {
public:
    enum class Type {
        None = 0x00,
        CurveConfig = 0x01,     //.vc
        RawData = 0x02,         //.rd
        ModeConfig = 0x04,      //.mc
        ProData = 0x08,         //.pd
        Result = 0x10           //.rs
    };
private:
    //! \brief 四字节对齐
    char _magic[MAGIC_LEN];                   // 8 byte
    unsigned int _crc_sum;                      // 4 byte
    unsigned int _type;                       // 4 byte
    char _info[INFO_LEN];                     // 48 byte
    unsigned int _time;                       // 4 byte
    char _reserved[RESERVED_LEN];             // 128 byte
public:
    Header();

    Header(unsigned int time, Type type);

    Header(const Header &header) = default;

    ~Header() = default;

    //getter and setter
    void setMagic(const char *magic);

    void setCrc32(unsigned int crc32);

    void setType(Type type);

    void setInfo(const QString &info);

    void setInfo(const char *info);

    void setTime(unsigned int time);

    void setReserved(const char *reserved);

    const char *magic() const;

    unsigned int crc32() const;

    unsigned int type() const;

    const char *info() const;

    unsigned int time() const;

    const char *reserved() const;

    char *magic();

    unsigned int &crc32();

    unsigned int &type();

    char *info();

    unsigned int &time();

    char *reserved();

    QStringList str() const;
};


#endif //REFINE_REF_H
