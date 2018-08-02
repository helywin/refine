//
// Created by jiang.wenqiang on 2018/7/4.
//

#include "Kebab.h"

Kebab::Cell::Cell() :
        size(KEBAB_CELL_LENGTH), head_index(0), tail_index(0), cell({}) {}

float &Kebab::Cell::operator[](unsigned int index) {
    return cell[index];
}

float &Kebab::Cell::head() {
    return cell[head_index];
}

float &Kebab::Cell::tail() {
    return cell[tail_index];
}

void Kebab::Cell::inc() {
    if (head_index == size - 1) {
        head_index = 0;
    } else {
        head_index += 1;
    }
}

void Kebab::Cell::dec() {
    if (tail_index == size - 1) {
        tail_index = 0;
    } else {
        tail_index += 1;
    }
}

bool Kebab::Cell::full() {
    return length() >= size - 1;
}

bool Kebab::Cell::empty() {
    return head_index == tail_index;
}

unsigned int Kebab::Cell::length() {
    int l = head_index - tail_index;
    if (l < 0) {
        l += size;
    }
    return (unsigned int) l;
}


Kebab::Kebab(unsigned short size) : cell_size(size), max_len(0) {
    cells = new Cell[size];
}

Kebab::~Kebab() {
    delete [] cells;
}

bool Kebab::add(unsigned short index, double val) {
    if (index > cell_size - 1) {
        return false;
    }
    if (cells[index].full()) {
        return false;
    } else {
        if (max_len < cells[index].length()) {
            max_len = cells[index].length();
        }
        cells[index].head() = val;
        cells[index].inc();
    }
    return true;
}

bool Kebab::out(double *list) {
    for (int i = 0; i < cell_size; ++i) {
        if (!cells[i].empty()) {
            list[i] = cells[i].tail();
            cells[i].dec();
        } else {
            return false;
        }
    }
    return true;
}

unsigned short Kebab::size() const {
    return cell_size;
}

unsigned int Kebab::length() const {
    return max_len;
}

bool Kebab::add(Kebab::Group &&group) {
    if (group.empty()) {
        return false;
    }
    for(const auto &key : group.keys()) {
        add(key, group[key]);
    }
    return true;
}
