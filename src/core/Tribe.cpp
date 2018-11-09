/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/11
 * @brief 曲线数据缓冲区
 * @details 存储曲线数据缓冲区
 ******************************************************************************/

#include <QtCore/QDataStream>
#include "Tribe.hpp"
#include "Csv.hpp"

/*!
 * @brief 曲线剪切
 * @deprecated 曲线设计另外的记录方式，该函数弃用
 *//*
void Tribe::trim()
{
    int size = -1;
    for (const auto &iter : _cells) {
        if (size == -1) {
            size = iter.size();
            continue;
        }
        if (size > iter.size()) {
            size = iter.size();
        }
    }
    for (auto &iter : _cells) {
        iter.resize(size);
    }
}*/

QDataStream &operator<<(QDataStream &stream, const Tribe &tribe)
{
    stream << tribe.size();
    stream << tribe.len();
    stream << (int) 0;
    stream << (int) 0;
    stream << (int) 0;
    stream << (int) 0;
    for (const auto &iter : tribe._styles) {
        stream << iter;
    }
    stream << tribe._segment.size();
    for (const auto &iter : tribe._segment) {
        stream << iter;
    }
    for (const auto &iter : tribe._cells) {
        stream << iter;
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Tribe &tribe)
{
    int size = 0;
    int reserved = 0;
    stream >> size;
    stream >> tribe._len;
    stream >> reserved;
    stream >> reserved;
    stream >> reserved;
    stream >> reserved;
    for (int i = 0; i < size; ++i) {
        Tribe::Style style;
        stream >> style;
        tribe._header.append(style.name());
        tribe._styles.append(qMove(style));
    }
    int segment_size;
    stream >> segment_size;
    for (int i = 0; i < segment_size; ++i) {
        int seg;
        stream >> seg;
        tribe._segment.append(seg);
    }
    for (int i = 0; i < size; ++i) {
        Tribe::Cell cell(tribe._header[i]);
        stream >> cell;
        tribe._cells.append(cell);
    }
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const Tribe::Cell &cell)
{
    stream << cell._name;
    stream << cell._data_type;
    stream << cell._data.size();
    stream << (int) 0;
    stream << (int) 0;
    stream << (int) 0;
    for (const auto &iter : cell._data) {
        stream << iter;
    }
    for (const auto &iter : cell._fill) {
        stream << iter;
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Tribe::Cell &cell)
{
    int size = 0;
    int reserved = 0;
    stream >> cell._name;
    stream >> cell._data_type;
    stream >> size;
    stream >> reserved;
    stream >> reserved;
    stream >> reserved;
    for (int i = 0; i < size; ++i) {
        float buf = 0;
        stream >> buf;
        cell._data.append(buf);
    }
    for (int i = 0; i < size; ++i) {
        unsigned char buf = 0;
        stream >> buf;
        cell._fill.append(buf);
    }
    return stream;
}

float Tribe::Cell::fakePercent() const
{
    int cnt = 0;
    for (const auto iter : _fill) {
        if (iter != Data) {
            cnt += 1;
        }
    }
    return (float) cnt / (float) _fill.size();
}

void Tribe::Cell::push(const Tribe::FillType fill, const float &v)
{
    _fill.append(fill);
    _data.push_back(v);
    _fill_this = true;
    if (fill != FakeByZero) {
        _empty = false;
    }
}

Tribe::Tribe() :
        _len(0) {}

/*!
 * @brief 清空数据
 */
void Tribe::Cell::reset()
{
    _data.clear();
    _fill.clear();
    _fill_this = false;
    _empty = true;
}

bool Tribe::dumpToCsv(QFile &f) const
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
    csv.writeLine(_header);
    QStringList list;
    for (int i = 0; i < len(); ++i) {
        list.clear();
        for (const auto &iter : _cells) {
            list.append(QString::number(iter.data()[i]));
        }
        csv.writeLine(list);
    }
    return csv.finishWrite();
}

/*!
 * @brief 清空数据不清空曲线
 */
void Tribe::reset()
{
    for (auto &iter : _cells) {
        iter.reset();
    }
    _len = 0;
    _segment.clear();
}

void Tribe::setUnFilled()
{
    for (auto &cell : _cells) {
        cell._fill_this = false;
    }
}

Tribe::Style::Style()
{}

Tribe::Style::Style(const Curve::Cell &cell)
{
    *this = cell;
}

Tribe::Style &Tribe::Style::operator=(const Curve::Cell &cell)
{
    _index = cell._index;
    _display = cell._display;
    _name = cell._name;
    _unit = cell._unit;
    _width = cell._width;
    _color = cell._color;
    _range_out[0] = cell._range_out[0];
    _range_out[1] = cell._range_out[1];
    _remark = cell._remark;
    return *this;
}

QDataStream &operator<<(QDataStream &stream, const Tribe::Style &style)
{
    stream << style._index
           << style._display
           << style._name
           << style._unit
           << style._width
           << style._color
           << style._range_out[0]
           << style._range_out[1]
           << style._remark;
    for (const auto &res : style._reserved) {
        stream << res;
    }
    return stream;
}

QDataStream &operator>>(QDataStream &stream, Tribe::Style &style)
{
    stream >> style._index
           >> style._display
           >> style._name
           >> style._unit
           >> style._width
           >> style._color
           >> style._range_out[0]
           >> style._range_out[1]
           >> style._remark;
    for (auto &res : style._reserved) {
        stream >> res;
    }
    return stream;
}

void Tribe::genFromCurve(const Curve &curve)
{
    clear();
    for (const auto &cell : curve) {
        _styles.append(Style(cell));
        _cells.append(Cell(cell.name(), Tribe::RawData));
        _header.append(cell.name());
    }
}

