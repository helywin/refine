//
// Created by jiang.wenqiang on 2018/7/4.
//

#ifndef CORE_KEBAB_H
#define CORE_KEBAB_H

#define KEBAB_CELL_LENGTH 4096

#include <QtCore/QMap>

class Kebab {
public:
    typedef QMap <unsigned short, double> Group;

    class Cell;

private:
    Cell *cells;

    unsigned short cell_size;

    unsigned int max_len;

public:


    Kebab() = delete;

    explicit Kebab(unsigned short size);

    ~Kebab();

    bool add(unsigned short index, double val);

    bool add(Group &&group);

    bool out(double *list);

    unsigned short size() const;

    unsigned int length() const;

};

class Kebab::Cell {
public:
    unsigned int size;

private:
    double cell[KEBAB_CELL_LENGTH];

    unsigned int head_index;

    unsigned int tail_index;

public:
    Cell();

    double &operator[](unsigned int index);

    double &head();

    double &tail();

    void inc();

    void dec();

    bool full();

    bool empty();

    unsigned int length();
};


#endif //CORE_DATARAFT_H
