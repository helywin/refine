/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/11
 * @brief 曲线配置
 * @details 曲线配置的读写和其他功能
 ******************************************************************************/

#include <QtCore/QDebug>
#include <QtCore/QDataStream>
#include "Curve.hpp"
#include "Csv.hpp"
#include "File.hpp"
#include "Tribe.hpp"

QStringList Curve::str() const
{
    QStringList list;
    for (const auto &cell : _cells) {
        list.append(cell.str().join(QChar(',')));
    }
    return list;
}

bool Curve::loadFromCsv(QFile &f)
{
    if (f.isOpen()) {
        f.close();
    }
    Csv csv;
    csv.setFile(&f);
    if (!csv.startRead("gbk")) {
        qCritical("文件打开失败");
        return false;
    }
    QStringList header;
    csv.readLine(header);
    while (!csv.finishRead()) {
        QStringList list;
        csv.readLine(list);
        unsigned short i = 0;
        Cell cell(i);
        bool first = true;
        for (const auto &iter : header) {
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
    setInitialized(true);
    return true;
}

bool Curve::dumpToCsv(QFile &f) const
{
    if (f.isOpen()) {
        f.close();
    }
    Csv csv;
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
    _header.clear();
    setInitialized(false);
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

void Curve::insert(int index, const Curve::Cell &cell)
{
    _header.insert(index, cell.name());
    _cells.insert(index, cell);
}

void Curve::insert(Curve::Cell &&cell, int index)
{
    _header.insert(index, cell.name());
    _cells.insert(index, qMove(cell));
}

void Curve::remove(int index)
{
    _header.removeAt(index);
    _cells.removeAt(index);
}

void Curve::remove(const QString &name)
{
    _cells.removeAt(_header.indexOf(name));
    _header.removeOne(name);
}


QDataStream &operator<<(QDataStream &stream, const Curve &curve)
{
    stream << curve.size();
    for (auto &iter : curve._cells) {
        stream << iter;
    }
    char end[5] = "ENDF";
    stream.writeRawData(end, 4);
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Curve &curve)
{
    int size;
    stream >> size;
    for (int i = 0; i < size; ++i) {
        Curve::Cell cell;
        stream >> cell;
        curve.append(cell);
    }
    return stream;
}


Curve::Cell::Cell() : Cell(0) {}


Curve::Cell::Cell(int index) :
        _index((short) index),
        _display(false),
        _name(QString("未命名")),
        _unit(QString("无单位")),
        _width(1),
        _color(0xFFFFFF),
        _can_id(0x777),
        _zero_byte(-1),
        _high_byte(-1),
        _high_range({0, 7}),
        _low_byte(0),
        _low_range({0, 7}),
        _range_in({0, 100}),
        _range_out({0, 100}),
        _remark(QString("无"))
{}

bool Curve::Cell::check() const     //不准备用csv文件，方法也不写了
{
    return true;
}

QStringList Curve::Cell::str() const
{
    QStringList list;
    list.append(indexStr());
    list.append(displayStr());
    list.append(nameStr());
    list.append(unitStr());
    list.append(widthStr());
    list.append(colorStr());
    list.append(canIdStr());
    list.append(zeroByteStr());
    list.append(highByteStr());
    list.append(lowByteStr());
    list.append(rangeInStr());
    list.append(rangeOutStr());
    list.append(remarkStr());
    return list;
}

void Curve::Cell::setHighByteByStr(const QString &s)
{
    if (s == QString("无")) {
        _high_byte = -1;
    } else {
        QStringList list = s.split(QChar(';'));
        _high_byte = list[0].toShort();
        list = list[1].split(QChar('~'));
        _high_range[0] = static_cast<unsigned char> (list[0].toUShort());
        _high_range[1] = static_cast<unsigned char> (list[1].toUShort());
    }
}

void Curve::Cell::setLowByteByStr(const QString &s)
{
    QStringList list = s.split(QChar(';'));
    _low_byte = list[0].toShort();
    list = list[1].split(QChar('~'));
    _low_range[0] = static_cast<unsigned char> (list[0].toUShort());
    _low_range[1] = static_cast<unsigned char> (list[1].toUShort());
}

QDataStream &operator<<(QDataStream &stream, const Curve::Cell &cell)
{
    stream << cell._index
           << cell._display
           << cell._name
           << cell._unit
           << cell._width
           << cell._color
           << cell._can_id
           << cell._zero_byte
           << cell._high_byte
           << cell._high_range[0]
           << cell._high_range[1]
           << cell._low_byte
           << cell._low_range[0]
           << cell._low_range[1]
           << cell._range_in[0]
           << cell._range_in[1]
           << cell._range_out[0]
           << cell._range_out[1]
           << cell._remark;
    for (const auto &res : cell._reserved) {
        stream << res;
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Curve::Cell &cell)
{
    stream >> cell._index
           >> cell._display
           >> cell._name
           >> cell._unit
           >> cell._width
           >> cell._color
           >> cell._can_id
           >> cell._zero_byte
           >> cell._high_byte
           >> cell._high_range[0]
           >> cell._high_range[1]
           >> cell._low_byte
           >> cell._low_range[0]
           >> cell._low_range[1]
           >> cell._range_in[0]
           >> cell._range_in[1]
           >> cell._range_out[0]
           >> cell._range_out[1]
           >> cell._remark;
    for (auto &res : cell._reserved) {
        stream >> res;
    }
    return stream;
}

void Curve::updateHeader()
{
    _header.clear();
    for (const auto &cell : _cells) {
        _header.append(cell.name());
    }
}


