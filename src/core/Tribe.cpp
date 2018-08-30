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

    for (const auto &iter : _pro_data) {
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

    for (const auto &iter : _pro_data) {
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

int Tribe::proSize() const {
    return _pro_data.size();
}

int Tribe::wholeSize() const {
    return rawSize() + proSize();
}


void Tribe::setIndex(const QStringList &index, const Tribe::DataType type) {
    switch (type) {
        case DataType::Raw:
            _raw_index = index;
            for (auto i = 0; i < _raw_index.size(); ++i) {
                _raw_data.append(Cell());
            }
            break;
        case DataType::Pro:
            _pro_index = index;
            for (auto i = 0; i < _raw_index.size(); ++i) {
                _pro_data.append(Cell());
            }
            break;
    }
}


void Tribe::addSequence(const QStringList &v, const Tribe::DataType type) {
    switch (type) {
        case DataType::Raw:
            Q_ASSERT(_raw_index.size() == v.size());
            for (auto i = 0; i < _raw_index.size(); ++i) {
                //修改double和float这里要改
                _raw_data[i].append(v[i].toFloat());
            }
            break;
        case DataType::Pro:
            Q_ASSERT(_pro_index.size() == v.size());
            for (auto i = 0; i < _pro_index.size(); ++i) {
                _pro_data[i].append(v[i].toFloat());
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
        case DataType::Pro:
            _pro_index.append(name);
            _pro_data.append(cell);
            break;
    }
}

void Tribe::addWholeCurve(const QString &name, const float_u *data, int len,
                          Tribe::DataType type) {
    switch (type) {
        case DataType::Raw:
            _raw_index.append(name);
            _raw_data.append(Cell(data, len));
            break;
        case DataType::Pro:
            _pro_index.append(name);
            _pro_data.append(Cell(data, len));
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
        case DataType::Pro:
            Q_ASSERT(_pro_index.contains(name));
            _pro_data.removeAt(_raw_index.indexOf(name));
            _pro_index.removeOne(name);
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

Tribe::Cell &Tribe::pro(const QString &name) {
    Q_ASSERT(_pro_index.contains(name));
    return _pro_data[_pro_index.indexOf(name)];
}

const Tribe::Cell &Tribe::pro(const QString &name) const {
    Q_ASSERT(_pro_index.contains(name));
    return _pro_data[_pro_index.indexOf(name)];
}

QStringList Tribe::rawIndex() const {
    return _raw_index;
}

QStringList Tribe::proIndex() const {
    return _pro_index;
}

int Tribe::memory() const {
    int mem = 0;
    for (const auto &iter : _raw_data) {
        mem += iter.length() * sizeof(double);
    }

    for (const auto &iter : _pro_data) {
        mem += iter.length() * sizeof(double);
    }
    return qMove(mem);
}

bool Tribe::isAligned() const {
    return maxLen() == minLen();
}

Tribe::Cell::Cell(const float_u *data, int len) {
    _cell.resize(len);
    memcpy(_cell.data(), data, len * sizeof(double));
}

int Tribe::Cell::length() const {
    return _cell.size();
}

float_u *Tribe::Cell::data(int &len) {
    len = _cell.size();
    return _cell.data();
}

const float_u *Tribe::Cell::data(int &len) const {
    len = _cell.size();
    return _cell.data();
}

float_u & Tribe::Cell::operator[](int index) {
    Q_ASSERT(index < _cell.size() && index >= 0);
    return _cell[index];
}

const float_u & Tribe::Cell::operator[](int index) const {
    Q_ASSERT(index < _cell.size() && index >= 0);
    return _cell[index];
}

void Tribe::Cell::append(float_u &v) {
    _cell.append(v);
}

void Tribe::Cell::append(float_u &&v) {
    _cell.append(v);
}
