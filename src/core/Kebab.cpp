//
// Created by jiang.wenqiang on 2018/8/8.
//

#include "Kebab.h"

Kebab::Kebab(const Curve::Header &header, Dump *dump, int cell_len) :
        _index(), _cells(), _cell_whole_size(cell_len), _dump(dump) {
    Q_ASSERT(dump != nullptr);
    for (const auto &iter : header) {
        addCell(iter);
    }
    _dump->setKebab(this);
}

Kebab::Cell &Kebab::operator[](int index) {
    Q_ASSERT(index >= 0 && index < _cells.size());
    return _cells[index];
}

Kebab::Cell &Kebab::operator[](const QString &name) {
    Q_ASSERT(_index.contains(name));
    return _cells[_index.indexOf(name)];
}

int Kebab::maxLen() const {
    int len = 0;
    for (const auto &iter : _cells) {
        if (len < iter.len()) {
            len = iter.len();
        }
    }
    return len;
}

int Kebab::minLen() const {
    int len = 0;
    for (const auto &iter : _cells) {
        if (len > iter.len()) {
            len = iter.len();
        }
    }
    return len;
}

void Kebab::addCell(const QString &name) {
    Q_ASSERT(!_index.contains(name));
    _index.append(name);
    Cell cell;
    cell.initialize(_cell_whole_size);
    _cells.append(qMove(cell));

}

void Kebab::removeCell(const QString &name) {
    Q_ASSERT(_index.contains(name));
    _cells.remove(_index.indexOf(name));
    _index.remove(_index.indexOf(name));
}

void Kebab::removeCell(int index) {
    Q_ASSERT(index < _index.size());
    _cells.remove(index);
    _index.remove(index);
}

bool Kebab::push(int index, const double &val) {
    Q_ASSERT(index < _cells.size());
    if (_cells[index].isFull()) {
        return false;
    }
    _cells[index].push(val);
    dump();
    return true;
}

bool Kebab::push(const QString &name, const double &val) {
    Q_ASSERT(_index.contains(name));
    if (_cells[_index.indexOf(name)].isFull()) {
        return false;
    }
    _cells[_index.indexOf(name)].push(val);
    dump();
    return true;
}

bool Kebab::isFull() const {
    for (const auto &iter : _cells) {
        if (iter.isFull()) {
            return true;
        }
    }
    return false;
}

bool Kebab::isEmpty() const {
    for (const auto &iter : _cells) {
        if (iter.isEmpty()) {
            return true;
        }
    }
    return false;
}

bool Kebab::pop(int index, float_u &val) {
    Q_ASSERT(index < _cells.size());
    if (_cells[index].isEmpty()) {
        return false;
    }
    _cells[index].pop(val);
    return true;
}

bool Kebab::pop(const QString &name, float_u &val) {
    Q_ASSERT(_index.contains(name));
    if (_cells[_index.indexOf(name)].isFull()) {
        return false;
    }
    _cells[_index.indexOf(name)].pop(val);
    return true;
}

bool Kebab::popLine(QStringList &list) {
    list.clear();
    float_u v;
    for (int i = 0; i < size(); ++i) {
        if (pop(i, v)) {
            list.append(QString::number(v));
        } else {
            return false;
        }
    }
    return true;
}

int Kebab::size() const {
    return _cells.size();
}

void Kebab::dump() {
    if (maxLen() > _cell_whole_size / 2) {
        _dump->start();
    }
}

int Kebab::wholeLen() const {
    return _cell_whole_size;
}

Kebab::Cell::Cell() :
        _cell(nullptr), _is_alloc(false),
        _head(-1), _tail(-1), _whole_size(0) {}

Kebab::Cell::~Cell() {
    if (_is_alloc) {
        delete[] _cell;
    }
}

void Kebab::Cell::initialize(int len) {
    if (_is_alloc) {
        return;
    }
    _cell = new float_u[len];
    _head = 0;
    _tail = 0;
    _is_alloc = true;
    _whole_size = len;
}

bool Kebab::Cell::push(const float_u &val) {
    if (isFull()) {
        return false;
    }
    _cell[_head] = val;
    headForward();
    return true;
}

bool Kebab::Cell::pop(float_u &val) {
    if (isEmpty()) {
        return false;
    }
    val = _cell[_tail];
    tailForward();
    return true;
}

bool Kebab::Cell::isFull() const {
    return _head - _tail == -1 ||
           _head - _tail == _whole_size - 1;
}

bool Kebab::Cell::isEmpty() const {
    return _head == _tail;
}

int Kebab::Cell::len() const {
    if (_head >= _tail) {
        return _head - _tail;
    } else {
        return _head - _tail + _whole_size;
    }
}

void Kebab::Cell::headForward() {
    if (_head < _whole_size - 1) {
        _head += 1;
    } else {
        _head = 0;
    }
}

void Kebab::Cell::tailForward() {
    if (_tail < _whole_size - 1) {
        _tail += 1;
    } else {
        _tail = 0;
    }
}


