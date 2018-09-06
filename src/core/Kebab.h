//
// Created by jiang.wenqiang on 2018/8/8.
//

#ifndef REFINE_KEBAB_H
#define REFINE_KEBAB_H

#include <QtCore/QVector>
#include "Define.h"
#include "Curve.h"
#include "Dump.h"

#define KEBAB_CELL_LENGTH 4096

class Kebab {
public:
    class Cell;

private:
    QVector<QString> _index;
    QVector<Cell> _cells;
    int _cell_whole_size;
    Dump *_dump;

public:
    Kebab() = delete;

    explicit Kebab(const Curve::Header &header, Dump *dump,
                   int cell_len = KEBAB_CELL_LENGTH);

    Cell &operator[](int index);

    Cell &operator[](const QString &name);

    int maxLen() const;

    int minLen() const;

    void addCell(const QString &name);

    void removeCell(const QString &name);

    void removeCell(int index);

    bool push(int index, const double &val);

    bool push(const QString &name, const double &val);

    bool isFull() const;

    bool isEmpty() const;

    int size() const;

    int wholeLen() const;

    bool popLine(QStringList &list);

private:
    void dump();

    bool pop(int index, float_u &val);

    bool pop(const QString &name, float_u &val);
};

class Kebab::Cell {
private:
    float_u *_cell;
    bool _is_alloc;
    int _head;
    int _tail;
    int _whole_size;

public:
    Cell();

    Cell(const Cell &v) = default;

    Cell &operator=(const Cell &v) = default;

    ~Cell();

    void initialize(int len);

    bool push(const float_u &val);

    bool pop(float_u &val);

    bool isFull() const;

    bool isEmpty() const;

    int len() const;

private:
    void headForward();

    void tailForward();
};


#endif //REFINE_KEBAB_H
