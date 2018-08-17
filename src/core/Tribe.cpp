//
// Created by jiang.wenqiang on 2018/7/10.
//

#include "Tribe.h"

int Tribe::maxLen() const {
    int size = 0;
    for(const auto &iter : _raw_data) {
        if (iter.length() > size) {
            size = iter.length();
        }
    }

    for(const auto &iter : _cal_data) {
        if (iter.length() > size) {
            size = iter.length();
        }
    }
    return qMove(size);
}

int Tribe::minLen() const {
    int size = 0;
    bool init = false;
    for(const auto &iter : _raw_data) {
        if (!init) {
            size = iter.length();
            init = true;
            continue;
        }
        if (iter.length() < size) {
            size = iter.length();
        }
    }

    for(const auto &iter : _cal_data) {
        if (!init) {
            size = iter.length();
            init = true;
            continue;
        }
        if (iter.length() > size) {
            size = iter.length();
        }
    }
    return qMove(size);
}

int Tribe::length() const {
    return minLen();
}

int Tribe::rawSize() const {
    return _raw_data.size();
}

int Tribe::calSize() const {
    return _cal_data.size();
}

int Tribe::wholeSize() const {
    return rawSize() + calSize();
}

void Tribe::addRaw(const QString &name, Tribe::Cell &&cell) {
    _raw_index.append(name);
    _raw_data.append(cell);
}

void Tribe::addRaw(const QString &name, const double *data, int len) {
    _raw_index.append(name);
    _raw_data.append(Cell(data, len));
}

void Tribe::addCal(const QString &name, Tribe::Cell &&cell) {
    _cal_index.append(name);
    _cal_data.append(cell);
}

void Tribe::addCal(const QString &name, const double *data, int len) {
    _cal_index.append(name);
    _cal_data.append(Cell(data, len));
}

void Tribe::removeRaw(const QString &name) {
    Q_ASSERT(_raw_index.contains(name));
    _raw_data.removeAt(_raw_index.indexOf(name));
    _raw_index.removeOne(name);
}

void Tribe::removeCal(const QString &name) {
    Q_ASSERT(_cal_index.contains(name));
    _cal_data.removeAt(_cal_index.indexOf(name));
    _cal_index.removeOne(name);
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
    for(const auto &iter : _raw_data) {
        mem += iter.length() * sizeof(double);
    }

    for(const auto &iter : _cal_data) {
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
    return _cell.data();
}

const double *Tribe::Cell::data(int &len) const {
    return _cell.data();
}

double &Tribe::Cell::operator[](int index) {
    Q_ASSERT(index < _cell.size() && index > 0);
    return _cell[index];
}

const double &Tribe::Cell::operator[](int index) const {
    Q_ASSERT(index < _cell.size() && index > 0);
    return _cell[index];
}
