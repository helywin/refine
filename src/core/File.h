//
// Created by jiang.wenqiang on 2018/9/12.
//

#ifndef REFINE_FILE_H
#define REFINE_FILE_H

class QDataStream;
class QStringList;
class QString;
class QFile;
class QByteArray;
class Curve;
class Tribe;
class Modes;


class File
{
public:
    class Header;

private:
    unsigned int _crc32[256];
    Header *_header;
    QDataStream *_stream;

public:
    File();
    ~File();
    File(const File &f) = delete;
    File &operator=(const File &f) = delete;

public:
    unsigned int crc32(const QByteArray &array) const;
    unsigned int crc32(const char *array, int len) const;

private:
    void initTable();
    bool loadFileHeader();
    void dumpFileHeader();
    void dumpFileHeaderCrc32();

public:
    bool loadCurveConfig(QFile &file, Curve &curve);
    bool dumpCurveConfig(QFile &file, const Curve &curve);

private:
    void convertCurveConfigToRef(const Curve &curve);
    bool convertRefToCurveConfig(Curve &curve);


};




#define HEADER_MAGIC_L 8
#define HEADER_VERSION_L 8
#define HEADER_CRC32_L 4
#define HEADER_TYPE_L 4
#define HEADER_INFO_L 44
#define HEADER_TIME_L 4
#define HEADER_RESV_L 128

#define HEADER_L (HEADER_MAGIC_L + HEADER_VERSION_L + HEADER_CRC32_L + \
                  HEADER_TYPE_L + HEADER_INFO_L + HEADER_TIME_L + HEADER_RESV_L)


class File::Header
{
public:
    enum FileType
    {
        None = 0x00,
        CurveConfig = 0x01,     //.cvc
        FrameData = 0x02,       //.fmd
        RawData = 0x04,         //.rwd
        ModeConfig = 0x08,      //.mdc
        ProData = 0x10,         //.prd
        Result = 0x20           //.rst
    };

private:
    char _magic[HEADER_MAGIC_L];                // 8 byte
    char _version[HEADER_VERSION_L];            // 8 byte
    unsigned int _crc_sum;                      // 4 byte
    unsigned int _type;                         // 4 byte
    char _info[HEADER_INFO_L];                  // 48 byte
    unsigned int _time;                         // 4 byte
    char _reserved[HEADER_RESV_L];              // 128 byte

public:
    Header();
    Header(unsigned int time, FileType type);
    Header(const Header &header) = default;
    ~Header() = default;

public:
    void setMagic(const char *magic);
    void setMagic();
    void setVersion(const char *version);
    void setVersion();
    void setCrc32(unsigned int crc32);
    void setType(FileType type);
    void setInfo(const QString &info);
    void setInfo(const char *info);
    void setTime(unsigned int time);
    void setReserved(const char *reserved);

    const char *magic() const;
    const char *version() const;
    unsigned int crc32() const;
    unsigned int type() const;
    const char *info() const;
    unsigned int time() const;
    const char *reserved() const;

    char *magic();
    char *version();
    unsigned int &crc32();
    unsigned int &type();
    char *info();
    unsigned int &time();
    char *reserved();
    QStringList str() const;
    bool check() const;
    void clear();
};

#endif //REFINE_FILE_H
