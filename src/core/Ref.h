//
// Created by jiang.wenqiang on 2018/8/15.
//

#ifndef REFINE_REF_H
#define REFINE_REF_H

#include <QtCore/QVector>

class QDataStream;

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
    Header *_header;
    QDataStream *_stream;

public:
    Ref();

    Ref(const Ref &ref) = delete;

    Ref &operator=(const Ref &ref) = delete;

private:
    void initTable();

    bool loadFileHeader();

    void dumpFileHeader();

    void dumpFileHeaderCrc32();

    //curve config
public:
    bool loadCurveConfig(QFile &file, Curve &curve);

    bool dumpCurveConfig(QFile &file, const Curve &curve);

private:
    void convertCurveConfigToRef(const Curve &curve);

    bool convertRefToCurveConfig(Curve &curve);

    //raw data
public:
    bool loadRawData(QFile &file, Tribe &tribe);

    bool pickRawData(QFile &file, Tribe &tribe, const QVector<int> &index);

    bool beginDumpRawData(QFile &file, const Kebab &kebab);

    bool dumpRawData(const Kebab &kebab);

    bool finishDumpRawData(const Kebab &kebab);

private:
    void convertRawDataToRef(const Kebab &in);

    bool convertRefToRawData(Tribe &out);

public:
    bool loadModeConfig(QFile &file);

    bool dumpModeConfig(QFile &file);

    bool loadProData(QFile &file, Tribe &tribe);

    bool dumpProData(QFile &file, const Tribe &tribe);

    bool loadResult(QFile &file);

    bool dumpResult(QFile &file);

    unsigned int crc32(const QByteArray &array) const;

    unsigned int crc32(const char *array, int len) const;

};

#define HEADER_MAGIC_L 8
#define HEADER_CRC32_L 4
#define HEADER_TYPE_L 4
#define HEADER_INFO_L 44
#define HEADER_TIME_L 4
#define HEADER_RESV_L 128
#define HEADER_L (HEADER_MAGIC_L + HEADER_CRC32_L + HEADER_TYPE_L + \
                   HEADER_INFO_L + HEADER_TIME_L + HEADER_RESV_L)

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
    char _magic[HEADER_MAGIC_L];                // 8 byte
    unsigned int _crc_sum;                      // 4 byte
    unsigned int _type;                         // 4 byte
    char _info[HEADER_INFO_L];                  // 48 byte
    unsigned int _time;                         // 4 byte
    char _reserved[HEADER_RESV_L];              // 128 byte
public:
    Header();

    Header(unsigned int time, Type type);

    Header(const Header &header) = default;

    ~Header() = default;

    //getter and setter
    void setMagic(const char *magic);

    void setMagic();

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

    bool check() const;

    void clear();
};

#endif //REFINE_REF_H
