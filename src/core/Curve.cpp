//
// Created by jiang.wenqiang on 2018/9/11.
//

#include <QtCore/QDebug>
#include <QtCore/QDataStream>
#include "Curve.hpp"
#include "Csv.hpp"
#include "File.hpp"

Curve::Curve() : _status(Status::Uninitialized) {}

QStringList Curve::str() const
{
    QStringList list;
    for (const auto &cell : _cells) {
        list.append(cell.str().join(QChar(',')));
    }
    return qMove(list);
}

void Curve::appendRow()
{
    _cells.append(Cell(_cells.size() + 1));
    _header.append(QString("未命名"));
}

void Curve::appendRow(const Bundle bundle)
{
    _cells.append(Cell(_cells.size() + 1, bundle));
    switch (bundle) {
        case Bundle::Acceleration:
            _header.append(QString("加速度"));
            break;
        default:
            break;
    }
}

void Curve::insertRow(const int index)
{
    if (index > _cells.size()) {
        _cells.insert(_cells.size(), Cell(_cells.size() + 1));
    } else if (index >= 0) {
        _cells.insert(index, Cell(index));
    }
}

void Curve::insertRow(const int index, const Bundle bundle)
{
    if (index > _cells.size()) {
        _cells.insert(_cells.size(), Cell(_cells.size() + 1, bundle));
    } else if (index >= 0) {
        _cells.insert(index, Cell(index, bundle));
    }
}

void Curve::removeRow(const int index)
{
    if (index > _cells.size()) {
        _cells.remove(_cells.size());
    } else if (index >= 0) {
        _cells.remove(index);
    }
}

bool Curve::loadFromCsv(QFile &f)
{
    if (f.isOpen()) {
        f.close();
    }
    ::Csv csv;
    csv.setFile(&f);
    if (!csv.startRead("gbk")) {
        qCritical("文件打开失败");
        return false;
    }
    csv.readLine(_table_head);
    while (!csv.finishRead()) {
        QStringList list;
        csv.readLine(list);
        unsigned short i = 0;
        Cell cell(i);
        bool first = true;
        for (const auto &iter : _table_head) {
            if (!first) {
                i += 1;
            } else {
                first = false;
            }
            if (iter == "序号") {
                cell.setIndexByStr(list[i]);
                continue;
            }
            if (iter == "显示") {
                cell.setDisplayByStr(list[i]);
                continue;
            }
            if (iter == "名称") {
                cell.setNameByStr(list[i]);
                _header.append(list[i]);
                continue;
            }
            if (iter == "类型") {
                cell.setTypeByStr(list[i]);
                continue;
            }
            if (iter == "单位") {
                cell.setUnitByStr(list[i]);
                continue;
            }
            if (iter == "宽度") {
                cell.setWidthByStr(list[i]);
                continue;
            }
            if (iter == "颜色") {
                cell.setColorByStr(list[i]);
                continue;
            }
            if (iter == "地址") {
                cell.setCanIdByStr(list[i]);
                continue;
            }
            if (iter == "零字节") {
                cell.setZeroByteByStr(list[i]);
                continue;
            }
            if (iter == "高字节") {
                cell.setHighByteByStr(list[i]);
                continue;
            }
            if (iter == "低字节") {
                cell.setLowByteByStr(list[i]);
                continue;
            }
            if (iter == "采样") {
                cell.setSampleByStr(list[i]);
                continue;
            }
            if (iter == "输入量程") {
                cell.setRangeInByStr(list[i]);
                continue;
            }
            if (iter == "输出量程") {
                cell.setRangeOutByStr(list[i]);
                continue;
            }
            if (iter == "备注") {
                cell.setRemarkByStr(list[i]);
                continue;
            }
        }
        if (cell.check()) {
            _cells.append(cell);
        } else {
            qCritical("曲线配置有问题");
            f.close();
            return false;
        }
    }
    _status = Status::Initialized;
    return true;
}

bool Curve::dumpToCsv(QFile &f) const
{
    if (f.isOpen()) {
        f.close();
    }
    ::Csv csv;
    csv.setFile(&f);
    if (!csv.startWrite("gbk")) {
        qCritical("文件打开失败");
        return false;
    }
    csv.writeLine(header());
    for (const auto &cell : _cells) {
        csv.writeLine(cell.str());
    }
    return csv.finishWrite();
}

void Curve::clear()
{
    _cells.clear();
    _table_head.clear();
    _header.clear();
    _status = Status::Uninitialized;
}

void Curve::append(const Curve::Cell &cell)
{
    _header.append(cell.name());
    _cells.append(cell);
}

void Curve::append(Curve::Cell &&cell)
{
    _header.append(cell.name());
    _cells.append(cell);
}

QDataStream &operator<<(QDataStream &stream, const Curve &curve)
{
    stream.device()->seek(HEADER_L);
    stream.writeRawData("CVCF", 4);
    stream << (unsigned int) curve.size();
    for (auto &iter : curve._cells) {
        stream << iter;
    }
    char end[5] = "ENDF";
    stream.writeRawData(end, 4);
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Curve &curve)
{
    stream.device()->seek(HEADER_L);
    char sign[4];
    stream.readRawData(sign, 4);
    if (sign[0] != 'C' || sign[1] != 'V' ||
        sign[2] != 'C' || sign[3] != 'F') {
        qCritical("bad file!");
    }
    unsigned short size;
    stream >> size;
    for (unsigned int i = 0; i < size; ++i) {
        Curve::Cell cell;
        stream >> cell;
        curve.append(cell);
    }
    return stream;
}


Curve::Cell::Cell() : Cell(0) {}

Curve::Cell::Cell(int index)
{
    Q_ASSERT(index >= 0);
    _index = (unsigned short) (index);
    _display = false;
    _name = QString("未命名");
    _type = Type::Physical;
    _unit = QString("无单位");
    _width = 1;
    _color = 0xFFFFFF;
    _can_id = 0x777;
    _zero_byte_existed = false;
    _zero_byte = 0;
    _high_byte_existed = false;
    _high_byte = 0;
    _high_byte_range[0] = 0;
    _high_byte_range[1] = 0;
    _low_byte = 0;
    _low_byte_range[0] = 0;
    _low_byte_range[1] = 7;
    _sample_type = Sample::Timed;
    _sample = 10;
    _range_in[0] = 0;
    _range_in[1] = 100;
    _range_out[0] = 0;
    _range_out[1] = 100;
//    _logic_map = ;
    _remark = QString("无");
    _bundle = Bundle::None;
}

Curve::Cell::Cell(int index, Curve::Bundle bundle) : Cell(index)
{
    switch (bundle) {
        case Bundle::Acceleration:
            _name = QString("加速度");
            _unit = QString("m/s^2");
            _remark = QString("汽车的加速度");
            break;
        default:
            break;
    }
}

bool Curve::Cell::check() const
{
    return true;
}

QStringList Curve::Cell::str() const
{
    QStringList list;
    list.append(indexStr());
    list.append(displayStr());
    list.append(nameStr());
    list.append(typeStr());
    list.append(unitStr());
    list.append(widthStr());
    list.append(colorStr());
    list.append(canIdStr());
    list.append(zeroByteStr());
    list.append(highByteStr());
    list.append(lowByteStr());
    list.append(sampleStr());
    list.append(rangeInStr());
    list.append(rangeOutStr());
    list.append(logicMapStr());
    list.append(remarkStr());
    return qMove(list);
}

QString Curve::Cell::logicMapStr() const
{
    if (_type == Type::Physical) {
        return QString("非逻辑变量");
    } else {
        QStringList list;
        for (const auto &iter : _logic_map.keys()) {
            list.append(QString("%1:%2").arg(iter).arg(_logic_map[iter]));
        }
        return qMove(list.join(QChar(';')));
    }
}


void Curve::Cell::setHighByteByStr(QString &s)
{
    if (s == QString("无")) {
        _high_byte_existed = false;
    } else {
        _high_byte_existed = true;
        QStringList list = s.split(QChar(';'));
        _high_byte = list[0].toUShort();
        list = list[1].split(QChar('~'));
        _high_byte_range[0] = list[0].toUShort();
        _high_byte_range[1] = list[1].toUShort();
    }
}

void Curve::Cell::setLowByteByStr(QString &s)
{
    QStringList list = s.split(QChar(';'));
    _low_byte = list[0].toUShort();
    list = list[1].split(QChar('~'));
    _low_byte_range[0] = list[0].toUShort();
    _low_byte_range[1] = list[1].toUShort();
}

void Curve::Cell::setSampleByStr(QString &s)
{
    QStringList list = s.split(QChar(';'));
    if (list[0] == QString("帧数")) {
        _sample_type = Sample::Framed;
    } else {
        _sample_type = Sample::Timed;
    }
    _sample = list[1].toULong();
}

QDataStream &operator<<(QDataStream &stream, const Curve::Cell &cell)
{
    stream << (unsigned char) cell._index;
    stream << (unsigned char) cell._display;
    stream << (unsigned char) cell._type;
    stream << (unsigned char) cell._width;

    char name[CURVE_NAME_L];
    memcpy(name, cell._name.toStdString().c_str(), CURVE_NAME_L);
    bool end = false;
    for (auto &iter : name) {
        if (iter == 0) {
            end = true;
        }
        if (end) {
            iter = 0;
        }
    }
    stream.writeRawData(name, CURVE_NAME_L);

    char unit[CURVE_UNIT_L];
    memcpy(unit, cell._unit.toStdString().c_str(), CURVE_UNIT_L);
    end = false;
    for (auto &iter : name) {
        if (iter == 0) {
            end = true;
        }
        if (end) {
            iter = 0;
        }
    }
    stream.writeRawData(unit, CURVE_UNIT_L);

    stream << (unsigned int) cell._color;
    stream << (unsigned int) cell._can_id;

    stream << (unsigned char) cell._zero_byte_existed;
    stream << (unsigned char) cell._zero_byte;
    stream << (unsigned char) 0 << (unsigned char) 0;

    stream << (unsigned char) cell._high_byte_existed;
    stream << (unsigned char) cell._high_byte;
    stream << (unsigned char) cell._high_byte_range[0];
    stream << (unsigned char) cell._high_byte_range[1];

    stream << (unsigned char) 0;
    stream << (unsigned char) cell._low_byte;
    stream << (unsigned char) cell._low_byte_range[0];
    stream << (unsigned char) cell._low_byte_range[1];

    stream << (unsigned char) cell._sample_type;
    stream << (unsigned char) 0;
    stream << (unsigned short) cell._sample;

    stream << (int) cell._range_in[0];
    stream << (int) cell._range_in[1];

    stream << (int) cell._range_out[0];
    stream << (int) cell._range_out[1];

    char remark[64];
    memcpy(remark, cell._remark.toStdString().c_str(), 64);
    end = false;
    for (auto &iter : name) {
        if (iter == 0) {
            end = true;
        }
        if (end) {
            iter = 0;
        }
    }
    stream.writeRawData(remark, 64);
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Curve::Cell &cell)
{
    unsigned char char_buf;
    unsigned char char_buf1;
    unsigned int uint_buf;
    int int_buf;
    int int_buf1;
    unsigned short short_buf;
    stream >> char_buf;
    cell.setIndexByVal(char_buf);
    stream >> char_buf;
    cell.setDisplayByVal(char_buf);
    stream >> char_buf;
    cell.setTypeByVal((Curve::Cell::Type) char_buf);
    stream >> char_buf;
    cell.setWidthByVal(char_buf);

    char name[CURVE_NAME_L];
    stream.readRawData(name, CURVE_NAME_L);
    cell.setNameByVal(QString::fromUtf8(name));

    char unit[CURVE_UNIT_L];
    stream.readRawData(unit, CURVE_UNIT_L);
    cell.setUnitByVal(QString::fromUtf8(unit));

    stream >> uint_buf;
    cell.setColorByVal(uint_buf);
    stream >> uint_buf;
    cell.setCanIdByVal(uint_buf);

    stream >> char_buf;
    cell.setZeroByteExistedByVal(char_buf);
    stream >> char_buf;
    cell.setZeroByteByVal(char_buf);
    stream >> char_buf;
    stream >> char_buf;

    stream >> char_buf;
    cell.setHighByteExistedByVal(char_buf);
    stream >> char_buf;
    cell.setHighByteByVal(char_buf);
    stream >> char_buf;
    stream >> char_buf1;
    cell.setHighByteRangeByVal(char_buf, char_buf1);

    stream >> char_buf;
    stream >> char_buf;
    cell.setLowByteByVal(char_buf);
    stream >> char_buf;
    stream >> char_buf1;
    cell.setLowByteRangeByVal(char_buf, char_buf1);

    stream >> char_buf;
    cell.setSampleTypeByVal((Curve::Cell::Sample) char_buf);
    stream >> char_buf;
    stream >> short_buf;
    cell.setSampleByVal(short_buf);

    stream >> int_buf;
    stream >> int_buf1;
    cell.setRangeInByVal(int_buf, int_buf1);

    stream >> int_buf;
    stream >> int_buf1;
    cell.setRangeOutByVal(int_buf, int_buf1);

    char remark[CURVE_REMARK_L];
    stream.readRawData(remark, CURVE_REMARK_L);
    cell.setRemarkByVal(QString::fromUtf8(remark));
    return stream;
}


