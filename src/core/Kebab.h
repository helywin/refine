//
// Created by jiang.wenqiang on 2018/7/4.
//

#ifndef CORE_KEBAB_H
#define CORE_KEBAB_H

#define RAFT_CELL_LENGTH 4096

class KebabCell {
public:
    unsigned int size;

private:
    double cell[RAFT_CELL_LENGTH];

    unsigned int head_index;

    unsigned int tail_index;

public:
    KebabCell();

    double &operator[](unsigned int index);

    double &head();

    double &tail();

    void inc();

    void dec();

    bool full();

    bool empty();

    unsigned int length();
};

class Kebab {
private:
    KebabCell *kebab;

    unsigned short cell_size;

    unsigned int max_len;

public:


    Kebab() = delete;

    explicit Kebab(unsigned short size);

    ~Kebab();

    bool add(unsigned short index, double val);

    bool out(double *list);

    unsigned short size();

    unsigned int length();

};


#endif //CORE_DATARAFT_H
