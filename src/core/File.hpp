/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/12
 * @brief 把数据转换为二进制文件的类
 * @details 附加相同的头，但不同数据存储方式不一样，还有CRC32检校码
 ******************************************************************************/

#ifndef REFINE_FILE_HPP
#define REFINE_FILE_HPP

#include <QtCore/QDataStream>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <windows.h>
#include <QtCore/QDateTime>
#include "Buffer.hpp"

class Curve;
class Tribe;
class Modes;
class FramePool;


/*!
 * @brief 把曲线配置、报文、数据、工况等等的文件读写类
 */

#define HEADER_START_POS 0
#define HEADER_MAGIC_POS HEADER_START_POS
#define HEADER_MAGIC_L 8
#define HEADER_VERSION_POS HEADER_MAGIC_L
#define HEADER_VERSION_L 8
#define HEADER_CRC32_POS (HEADER_VERSION_POS + HEADER_VERSION_L)
#define HEADER_CRC32_L 4
#define HEADER_TYPE_POS (HEADER_CRC32_POS + HEADER_CRC32_L)
#define HEADER_TYPE_L 4
#define HEADER_INFO_POS (HEADER_TYPE_POS + HEADER_TYPE_L)
#define HEADER_INFO_L 48
#define HEADER_BIRTH_POS (HEADER_INFO_POS + HEADER_INFO_L)
#define HEADER_BIRTH_L 4
#define HEADER_MODIFIED_POS (HEADER_BIRTH_POS + HEADER_BIRTH_L)
#define HEADER_MODIFIED_L 4
#define HEADER_RESV_POS (HEADER_MODIFIED_POS + HEADER_MODIFIED_L)
#define HEADER_RESV_L 128

#define HEADER_L (HEADER_MAGIC_L + HEADER_VERSION_L + HEADER_CRC32_L + \
                  HEADER_TYPE_L + HEADER_INFO_L + HEADER_BIRTH_L + \
                  HEADER_MODIFIED_L + HEADER_RESV_L)
#define DATA_POS HEADER_L

class File
{
public:
    class Header
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
        unsigned int _birth_time;                   // 4 byte
        unsigned int _modified_time;                // 4 byte
        char _reserved[HEADER_RESV_L];              // 128 byte

    public:
        Header();
        Header(unsigned int time, FileType type);
        Header(const Header &header) = default;
        ~Header() = default;

    public:
        inline void setMagic(const char *magic)
        {
            memcpy(_magic, magic, sizeof(_magic));
        }

        void setMagic();

        inline void setVersion(const char *version)
        {
            memcpy(_version, version, sizeof(_version));
        }

        void setVersion();

        inline void setCrc32(unsigned int crc32) { _crc_sum = crc32; }

        inline void setType(FileType type) { _type = type; }

        void setInfo(const QString &info);
        void setInfo(const char *info);
        void setInfo();

        inline void setBirth(unsigned int birth) { _birth_time = birth; }

        inline void
        setBirth() { _birth_time = QDateTime::currentDateTime().toTime_t(); }

        inline void
        setModified(unsigned int modified) { _modified_time = modified; }

        inline void
        setModified() { _modified_time = QDateTime::currentDateTime().toTime_t(); }

        inline void setReserved(const char *reserved)
        {
            memcpy(_reserved, reserved, sizeof(_reserved));
        }

        inline const char *magic() const { return _magic; }

        inline const char *version() const { return _version; }

        inline unsigned int crc32() const { return _crc_sum; }

        inline unsigned int type() const { return _type; }

        inline const char *info() const { return _info; }

        inline unsigned int birth() const { return _birth_time; }

        inline unsigned int modified() const { return _modified_time; }

        inline const char *reserved() const { return _reserved; }

        inline char *magic() { return _magic; }

        inline char *version() { return _version; }

        inline unsigned int &crc32() { return _crc_sum; }

        inline unsigned int &type() { return _type; }

        inline char *info() { return _info; }

        inline unsigned int &birth() { return _birth_time; }

        inline unsigned int &modified() { return _modified_time; }

        inline char *reserved() { return _reserved; }

        QStringList str() const;
        bool check() const;
        void clear();
        int versionCompare(char major, char micro, char minor);
    };

private:
    unsigned int _crc32[256];
    Header *_header;
    QDataStream *_stream;
    unsigned int _frame_obj_num;
    unsigned int _frame_cell_num;

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
private:
    void loadHeaderMagic();
    void loadHeaderVersion();
    void loadHeaderCrc32();
    void loadHeaderType();
    void loadHeaderInfo();
    void loadHeaderBirth();
    void loadHeaderModified();
    void loadHeaderReserved();
    bool loadCheckSum();

    void dumpHeaderMagic();
    void dumpHeaderVersion();
    void dumpHeaderCrc32();
    void dumpHeaderType();
    void dumpHeaderInfo();
    void dumpHeaderBirth();
    void dumpHeaderModified();
    void dumpHeaderReserved();

    void loadFileHeader();
    void dumpFileHeader();

public:
    bool loadCurveConfig(QFile &file, Curve &curve);
    bool dumpCurveConfig(QFile &file, const Curve &curve);

public:
    bool loadFrameRecord(QFile &file, FramePool &pool);

    bool dumpFrameRecordBegin(QFile &file);
    void dumpFrameRecord(Buffer &buffer, Buffer::Iter tail, Buffer::Iter head);
    void dumpFrameRecordFinish(QFile &file);

public:
    bool loadCurveRecord(QFile &file, Tribe &tribe);
    bool dumpCurveRecord(QFile &file, const Tribe &tribe);

};

#endif //REFINE_FILE_HPP
