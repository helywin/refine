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
    stream << tribe._cells.size();
    stream << (int) 0;
    stream << (int) 0;
    stream << (int) 0;
    for (const auto &iter : tribe._header) {
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
    stream >> reserved;
    stream >> reserved;
    stream >> reserved;
    for (int i = 0; i < size; ++i) {
        QString name;
        stream >> name;
        tribe._header.append(name);
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
    return (float)cnt / (float)_fill.size();
}

void Tribe::Cell::push(const Tribe::Fill fill, const float &v)
{
    _fill.append(fill);
    _data.push_back(v);
    _fill_this = true;
    if (fill != FakeByZero) {
        _empty = false;
    }
}

int Tribe::minLen() const
{
    int len = -1;
    for (const auto &iter : _cells) {
        if (len == -1) {
            len = iter.size();
            continue;
        }
        if (len > iter.size()) {
            len = iter.size();
        }
    }
    return len;
}


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

int Tribe::maxLen() const
{
    int len = -1;
    for (const auto &iter : _cells) {
        if (iter.size() > len) {
            len = iter.size();
        }
    }
    return len;
}

QStringList Tribe::zeroLenData() const
{
    QStringList list;
    for (const auto &iter : _cells) {
        if (iter.empty()) {
            list.append(iter.name());
        }
    }
    return list;
}

bool Tribe::loadFromCsv(QFile &f)
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
    csv.readLine(_header);
    append(_header);
    while (!csv.finishRead()) {
        QStringList list;
        csv.readLine(list);
        for (int i = 0; i < list.size(); ++i) {
            (*this)[_header[i]].push(Data, list[i].toFloat());
        }
    }
    return true;
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
    for (int i = 0; i < minLen(); ++i) {
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
    _segment.clear();
}

/*!
 * @brief 数据段间增加空白
 * @deprecated 在数据中加入段序号，此函数弃用
 */
void Tribe::addGap()
{
    for (auto &cell : _cells) {
        for (int i = 0; i < 100; ++i) {
            cell._data.append(0);
        }
    }
}

void Tribe::setUnFilled()
{
    for (auto &cell : _cells) {
        cell._fill_this = false;
    }
}
