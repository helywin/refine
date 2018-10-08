//
// Created by jiang.wenqiang on 2018/9/11.
//

#include <QtCore/QDataStream>
#include "Tribe.hpp"
#include "Csv.hpp"

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
}

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
    return qMove(len);
}

int Tribe::maxLen() const
{
    int len = -1;
    for (const auto &iter : _cells) {
        if (iter.size() > len) {
            len = iter.size();
        }
    }
    return qMove(len);
}

QStringList Tribe::zeroLenData() const
{
    QStringList list;
    for (const auto &iter : _cells) {
        if (iter.size() == 0) {
            list.append(iter.name());
        }
    }
    return qMove(list);
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
            (*this)[_header[i]].data().append(list[i].toFloat());
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

void Tribe::reset()
{
    for (auto &iter : _cells) {
        iter.reset();
    }
}

void Tribe::addGap()
{
    for (auto &cell : _cells) {
        for (int i = 0; i < 100; ++i) {
            cell.data().append(0);
        }
    }
}
