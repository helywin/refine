//
// Created by jiang.wenqiang on 2018/7/4.
//

#include "Kebab.h"

<<<<<<< HEAD
KebabCell::KebabCell() :
        size(RAFT_CELL_LENGTH), head_index(0), tail_index(0){}

double &KebabCell::operator[](unsigned int index) {
    return cell[index];
}

double &KebabCell::head() {
    return cell[head_index];
}

double &KebabCell::tail() {
=======
Kebab::Cell::Cell() :
        size(KEBAB_CELL_LENGTH), head_index(0), tail_index(0), cell({}) {}

float &Kebab::Cell::operator[](unsigned int index) {
    return cell[index];
}

float &Kebab::Cell::head() {
    return cell[head_index];
}

float &Kebab::Cell::tail() {
>>>>>>> 568e96c... 准备重新开发结构
    return cell[tail_index];
}

void KebabCell::inc() {
    if(head_index == size - 1) {
        head_index = 0;
    } else {
        head_index += 1;
    }
}

void KebabCell::dec() {
    if(tail_index == size - 1) {
        tail_index = 0;
    } else {
        tail_index += 1;
    }
}

bool KebabCell::full() {
    return length() >= size - 1;
}

bool KebabCell::empty() {
    return head_index == tail_index;
}

unsigned int KebabCell::length() {
    int l = head_index - tail_index;
    if (l < 0) {
        l += size;
    }
    return (unsigned int)l;
}


Kebab::Kebab(unsigned short size) : cell_size(size), max_len(0) {
    kebab = new KebabCell[size];
}

Kebab::~Kebab() {
    delete kebab;
}

bool Kebab::add(unsigned short index, double val) {

    if (kebab[index].full()) {
        return false;
    } else {
        if (max_len < kebab[index].length()) {
            max_len = kebab[index].length();
        }
        kebab[index].head() = val;
        kebab[index].inc();
    }
    return true;
}

bool Kebab::out(double *list) {
    for (int i = 0; i < cell_size; ++ i) {
        if (!kebab[i].empty()) {
            list[i] = kebab[i].tail();
            kebab[i].dec();
        } else {
            return false;
        }
    }
    return true;
}

unsigned short Kebab::size() {
    return cell_size;
}

unsigned int Kebab::length() {
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
