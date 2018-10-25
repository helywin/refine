/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/12
 * @brief 把数据转换为二进制文件的类
 * @details 附加相同的头，但不同数据存储方式不一样，还有CRC32检校码
 ******************************************************************************/

#include "File.hpp"
#include "Version.hpp"
#include "Curve.hpp"
#include "Tribe.hpp"


File::File()
        : _header(new File::Header()),
          _stream(new QDataStream()),
          _frame_obj_num(0),
          _frame_cell_num(0)
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

void File::loadHeaderMagic()
{
    _stream->device()->seek(HEADER_MAGIC_POS);
    _stream->readRawData(_header->magic(), HEADER_MAGIC_L);
}

void File::loadHeaderVersion()
{
    _stream->device()->seek(HEADER_VERSION_POS);
    _stream->readRawData(_header->version(), HEADER_VERSION_L);
}

void File::loadHeaderCrc32()
{
    _stream->device()->seek(HEADER_CRC32_POS);
    (*_stream) >> _header->crc32();
}

void File::loadHeaderType()
{
    _stream->device()->seek(HEADER_TYPE_POS);
    (*_stream) >> _header->type();
}

void File::loadHeaderInfo()
{
    _stream->device()->seek(HEADER_INFO_POS);
    _stream->readRawData(_header->info(), HEADER_INFO_L);
}

void File::loadHeaderBirth()
{
    _stream->device()->seek(HEADER_BIRTH_POS);
    (*_stream) >> _header->birth();
}

void File::loadHeaderModified()
{
    _stream->device()->seek(HEADER_MODIFIED_POS);
    (*_stream) >> _header->modified();
}

void File::loadHeaderReserved()
{
    _stream->device()->seek(HEADER_RESV_POS);
    _stream->readRawData(_header->reserved(), HEADER_RESV_L);
}

bool File::loadCheckSum()
{
    QByteArray byte_array;
    signed char byte;
    unsigned int checksum;
     _stream->device()->seek(HEADER_CRC32_POS);
    (*_stream) >> checksum;
    while (!_stream->atEnd()) {
        (*_stream) >> byte;
        byte_array.append(byte);
    }
    unsigned int check = crc32(byte_array);
    return check == checksum;
}

void File::dumpHeaderMagic()
{
    _stream->device()->seek(HEADER_MAGIC_POS);
    _stream->writeRawData(_header->magic(), HEADER_MAGIC_L);
}

void File::dumpHeaderVersion()
{
    _stream->device()->seek(HEADER_VERSION_POS);
    _stream->writeRawData(_header->version(), HEADER_VERSION_L);
}

void File::dumpHeaderCrc32()
{
    _stream->device()->seek(HEADER_CRC32_POS + HEADER_CRC32_L);
    QByteArray byte_array;
    signed char byte;
    byte_array.clear();
    while (!_stream->atEnd()) {
        (*_stream) >> byte;
        byte_array.append(byte);
    }
    unsigned int check = crc32(byte_array);
    _stream->device()->seek(HEADER_CRC32_POS);
    (*_stream) << check;
}

void File::dumpHeaderType()
{
    _stream->device()->seek(HEADER_TYPE_POS);
    (*_stream) << _header->type();
}

void File::dumpHeaderInfo()
{
    _stream->device()->seek(HEADER_INFO_POS);
    _stream->writeRawData(_header->info(), HEADER_INFO_L);
}

void File::dumpHeaderBirth()
{
    _stream->device()->seek(HEADER_BIRTH_POS);
    (*_stream) << _header->birth();
}

void File::dumpHeaderModified()
{
    _stream->device()->seek(HEADER_MODIFIED_POS);
    (*_stream) << _header->modified();
}

void File::dumpHeaderReserved()
{
    _stream->device()->seek(HEADER_RESV_POS);
    _stream->writeRawData(_header->reserved(), HEADER_RESV_L);
}

void File::loadFileHeader()
{
    loadHeaderMagic();
    loadHeaderVersion();
    loadHeaderType();
    loadHeaderInfo();
    loadHeaderBirth();
    loadHeaderModified();
    loadHeaderReserved();
}

void File::dumpFileHeader()
{
    dumpHeaderMagic();
    dumpHeaderVersion();
    dumpHeaderType();
    dumpHeaderInfo();
    dumpHeaderBirth();
    dumpHeaderModified();
    dumpHeaderReserved();
}

bool File::loadCurveConfig(QFile &file, Curve &curve)
{
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen()) {
        return false;
    }
    _stream->setDevice(&file);

    loadFileHeader();
    if (!loadCheckSum()) {
        return false;
    }
    file.seek(DATA_POS);
    (*_stream) >> curve;
    curve.genSubIdMap777();
    curve.genOtherIdMap();
    curve.setInitialized(true);
    _stream->unsetDevice();
    file.close();
    return true;
}

bool File::dumpCurveConfig(QFile &file, const Curve &curve)
{
    _header->clear();
    if (file.exists()) {
        file.open(QIODevice::ReadOnly);
        if (file.isOpen()) {
            _stream->setDevice(&file);
            loadFileHeader();
            _stream->unsetDevice();
            file.close();
        }
    } else {
        _header->setBirth();
    }
    file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    if (!file.isOpen()) {
        qDebug("打开文件失败！");
        return false;
    }
    _stream->setDevice(&file);
    _header->setMagic();
    _header->setVersion();
    _header->setModified();
    _header->setType(Header::FileType::CurveConfig);
    _header->setInfo();

    dumpFileHeader();

    file.seek(DATA_POS);

    (*_stream) << curve;

    dumpHeaderCrc32();

    _stream->unsetDevice();
    file.close();

    return true;
}

bool File::loadFrameRecordBegin(QFile &file, Buffer &buffer, int *pack, int *frame)
{
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen()) {
        return false;
    }
    _stream->setDevice(&file);

    loadFileHeader();
    if(!loadCheckSum()) {
        qCritical("File::loadFrameRecordBegin CRC32检校失败");
        return false;
    }
    _stream->device()->seek(DATA_POS);
    char str[4];
    _stream->readRawData(str, 4);
    (*_stream) >> _frame_obj_num;
    (*_stream) >> _frame_cell_num;
    if (pack) {
        *pack = _frame_cell_num;
    }
    if (frame) {
        *frame = _frame_obj_num;
    }
    buffer.reset();
    return true;
}

bool File::loadFrameRecord(Buffer &buffer)
{
    (*_stream) >> buffer;
    if (_stream->atEnd()) {
        return false;
    }
    char sign[4];
    _stream->readRawData(sign, 4);
    if (sign[0] == 'E' && sign[1] == 'N' &&
            sign[2] == 'D' && sign[3] == 'F') {
        return false;   //读取完毕
    } else {
        _stream->device()->seek(_stream->device()->pos() - 4);
        return true;
    }
}

void File::loadFrameRecordFinish(QFile &file)
{
    _stream->unsetDevice();
    file.close();
}

bool File::dumpFrameRecordBegin(QFile &file)
{
    file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    if (!file.isOpen()) {
        qDebug("false");
        return false;
    }
    _stream->setDevice(&file);
    _header->setMagic();
    _header->setVersion();
    _header->setBirth();
    _header->setModified();
    _header->setType(Header::FileType::FrameData);
    _header->setInfo();

    dumpFileHeader();
    _frame_obj_num = 0;
    _frame_cell_num = 0;
    _stream->writeRawData("FMDF", 4);
    (*_stream) << _frame_obj_num
               << _frame_cell_num;
    return true;
}

void File::dumpFrameRecord(Buffer &buffer, Buffer::Iter tail, Buffer::Iter head)
{
    int cell_n;
    int obj_n;
    buffer.size(cell_n, obj_n);
    _frame_cell_num += cell_n;
    _frame_obj_num += obj_n;
    buffer.dump(*_stream, tail, head);
    auto pos = _stream->device()->pos();
    _stream->device()->seek(DATA_POS + 4);
    (*_stream) << _frame_obj_num
               << _frame_cell_num;
    _stream->device()->seek(pos);
}

void File::dumpFrameRecordFinish(QFile &file)
{
    _stream->writeRawData("ENDF", 4);
    _stream->device()->seek(DATA_POS + 4);
    (*_stream) << _frame_obj_num
               << _frame_cell_num;
    dumpHeaderCrc32();
    _stream->unsetDevice();
    file.close();
}

bool File::loadCurveRecord(QFile &file, Tribe &tribe)
{
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen()) {
        return false;
    }
    _stream->setDevice(&file);

    loadFileHeader();
    if(!loadCheckSum()) {
        qDebug("bad checksum!");
        return false;
    }
    _stream->device()->seek(DATA_POS);
    char str[4];
    _stream->readRawData(str, 4);
    tribe.reset();
    (*_stream) >> tribe;
    return true;
}

bool File::dumpCurveRecord(QFile &file, const Tribe &tribe)
{
    file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    if (!file.isOpen()) {
        qDebug("false");
        return false;
    }
    _stream->setDevice(&file);
    _header->setMagic();
    _header->setVersion();
    _header->setBirth();
    _header->setModified();
    _header->setType(Header::FileType::RawData);
    _header->setInfo();

    dumpFileHeader();
    _stream->writeRawData("RPDF", 4);
    (*_stream) << tribe;
    return true;
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
    _birth_time = time;
    _modified_time = time;
    _type = static_cast<unsigned int> (type);
    memset(_reserved, '\0', sizeof(_reserved));
    _reserved[HEADER_RESV_L - 1] = static_cast<char> (0xFF);
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
    _magic[7] = '0';
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

void File::Header::setInfo()
{
    char user[50];
    char computer[50];
    unsigned long user_l = 50;
    unsigned long computer_l = 50;
    GetUserNameA(user, &user_l);
    GetComputerNameA(computer, &computer_l);
    if (user_l > HEADER_INFO_L) {
        memcpy(_info, user, sizeof(_info));
        return;
    } else {
        memcpy(_info, user, sizeof(char) * user_l);
    }
    if (user_l < HEADER_INFO_L) {
        _info[user_l - 1] = '@';
    }
    if (user_l + computer_l > HEADER_INFO_L) {
        memcpy(_info + user_l, computer,
               sizeof(char) * (HEADER_INFO_L - user_l));
    } else {
        memcpy(_info + user_l, computer,
                sizeof(char) * computer_l);
        memset(_info + user_l + computer_l, 0,
                HEADER_INFO_L - user_l - computer_l);
    }
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
    QString date = QDateTime::fromTime_t(_birth_time).
            toString(QString("yyyy/MM/dd hh:mm:ss"));
    list.append(qMove(date));
    return list;
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
    _birth_time = 0;
    _modified_time = 0;
    memset(_reserved, 0, sizeof(_reserved));
    _reserved[HEADER_RESV_L - 1] = (char) 0xFF;
}

int File::Header::versionCompare(char major, char micro, char minor)
{
    if (_version[0] < major) {
        return -1;
    } else if (_version[0] > major){
        return 1;
    }
    if (_version[1] < micro) {
        return -1;
    } else if (_version[1] > micro){
        return 1;
    }
    if (_version[2] < minor) {
        return -1;
    } else if (_version[2] > minor){
        return 1;
    }
    return 0;
}
