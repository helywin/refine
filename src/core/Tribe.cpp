//
// Created by jiang.wenqiang on 2018/7/10.
//

#include "Tribe.h"


Tribe::Tribe(const QStringList &keys) {
    for (const auto &key : keys) {
        if (!key.isEmpty()) {
            clan.insert(key, Cell());
        }
    }
}

void Tribe::add(const QString &key) {
    clan.insert(key, Cell());
}

void Tribe::add(const QString &str, unsigned int size) {
    clan.insert(str, Cell(size));
}

Tribe::Cell &Tribe::operator[](const QString &key) {
    return clan[key];
}

QString Tribe::str(const QString &key) {
    QStringList list;
    for (const auto &iter : clan[key]) {
        list.append(QString::number(iter));
    }
<<<<<<< HEAD
    return list.join(QChar(','));
=======

}

Tribe::Cell &Tribe::raw(const QString &name) {
    Q_ASSERT(_raw_index.contains(name));
    return _raw_data[_raw_index.indexOf(name)];
}

const Tribe::Cell &Tribe::raw(const QString &name) const {
    Q_ASSERT(_raw_index.contains(name));
    return _raw_data[_raw_index.indexOf(name)];
}

Tribe::Cell &Tribe::cal(const QString &name) {
    Q_ASSERT(_cal_index.contains(name));
    return _cal_data[_cal_index.indexOf(name)];
}

const Tribe::Cell &Tribe::cal(const QString &name) const {
    Q_ASSERT(_cal_index.contains(name));
    return _cal_data[_cal_index.indexOf(name)];
}

QStringList Tribe::rawIndex() const {
    return _raw_index;
}

QStringList Tribe::calIndex() const {
    return _cal_index;
}

int Tribe::memory() const {
    int mem = 0;
    for (const auto &iter : _raw_data) {
        mem += iter.length() * sizeof(double);
    }

    for (const auto &iter : _cal_data) {
        mem += iter.length() * sizeof(double);
    }
    return qMove(mem);
}

bool Tribe::isAligned() const {
    return maxLen() == minLen();
}

Tribe::Cell::Cell(const double *data, int len) {
    _cell.resize(len);
    memcpy(_cell.data(), data, len * sizeof(double));
}

int Tribe::Cell::length() const {
    return _cell.size();
}

double *Tribe::Cell::data(int &len) {
    len = _cell.size();
    return _cell.data();
}

const double *Tribe::Cell::data(int &len) const {
    len = _cell.size();
    return _cell.data();
}

double &Tribe::Cell::operator[](int index) {
    Q_ASSERT(index < _cell.size() && index >= 0);
    return _cell[index];
}

const double &Tribe::Cell::operator[](int index) const {
    Q_ASSERT(index < _cell.size() && index >= 0);
    return _cell[index];
}

void Tribe::Cell::append(double &v) {
    _cell.append(v);
}

void Tribe::Cell::append(double &&v) {
    _cell.append(v);
>>>>>>> 07d1fd7... 发送数据小工具写完,还需测试
}
