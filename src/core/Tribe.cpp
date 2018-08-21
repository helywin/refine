//
// Created by jiang.wenqiang on 2018/7/10.
//

#include "Tribe.h"

int Tribe::maxLen() const {
    int size = 0;
    for (const auto &iter : _raw_data) {
        if (iter.length() > size) {
            size = iter.length();
        }
    }

    for (const auto &iter : _cal_data) {
        if (iter.length() > size) {
            size = iter.length();
        }
    }
    return qMove(size);
}

int Tribe::minLen() const {
    int size = 0;
    bool init = false;
    for (const auto &iter : _raw_data) {
        if (!init) {
            size = iter.length();
            init = true;
            continue;
        }
        if (iter.length() < size) {
            size = iter.length();
        }
    }

    for (const auto &iter : _cal_data) {
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


void Tribe::setIndex(const QStringList &index, const Tribe::DataType type) {
    switch (type) {
        case DataType::Raw:
            _raw_index = index;
            for (auto i = 0; i < _raw_index.size(); ++i) {
                _raw_data.append(Cell());
            }
            break;
        case DataType::Calculated:
            _cal_index = index;
            for (auto i = 0; i < _raw_index.size(); ++i) {
                _cal_data.append(Cell());
            }
            break;
    }
}


void Tribe::addSequence(const QStringList &v, const Tribe::DataType type) {
    switch (type) {
        case DataType::Raw:
            Q_ASSERT(_raw_index.size() == v.size());
            for (auto i = 0; i < _raw_index.size(); ++i) {
                _raw_data[i].append(v[i].toDouble());
            }
            break;
        case DataType::Calculated:
            Q_ASSERT(_cal_index.size() == v.size());
            for (auto i = 0; i < _cal_index.size(); ++i) {
                _cal_data[i].append(v[i].toDouble());
            }
            break;
    }
}

void Tribe::addWholeCurve(const QStringList &name, Cell &&cell,
                          const Tribe::DataType type) {
    switch (type) {
        case DataType::Raw:
            _raw_index.append(name);
            _raw_data.append(cell);
            break;
        case DataType::Calculated:
            _cal_index.append(name);
            _cal_data.append(cell);
            break;
    }
}

void Tribe::addWholeCurve(const QString &name, const double *data, int len,
                          Tribe::DataType type) {
    switch (type) {
        case DataType::Raw:
            _raw_index.append(name);
            _raw_data.append(Cell(data, len));
            break;
        case DataType::Calculated:
            _cal_index.append(name);
            _cal_data.append(Cell(data, len));
            break;
    }
}

void Tribe::removeWholeCurve(const QString &name, Tribe::DataType type) {
    switch (type) {
        case DataType::Raw:
            Q_ASSERT(_raw_index.contains(name));
            _raw_data.removeAt(_raw_index.indexOf(name));
            _raw_index.removeOne(name);
            break;
        case DataType::Calculated:
            Q_ASSERT(_cal_index.contains(name));
            _cal_data.removeAt(_raw_index.indexOf(name));
            _cal_index.removeOne(name);
            break;
    }

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
}
