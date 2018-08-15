//
// Created by jiang.wenqiang on 2018/8/8.
//

#ifndef REFINE_KEBAB_H
#define REFINE_KEBAB_H

#include <QtCore/QVector>
#include <QtCore/QThread>
#include "Curve.h"
#include "Dump.h"

#define KEBAB_CELL_LENGTH 4096

class Kebab : public QThread{
Q_OBJECT
public:
    class Cell;

private:
    QVector<QString> _index;
    QVector<Cell> _cells;
    int _cell_whole_size;
    Dump *_dump;

public:
    Kebab() = delete;
    explicit Kebab(const Curve::Header &header, Dump *dump, int cell_len = KEBAB_CELL_LENGTH);
    Cell &operator[](int index);
    Cell &operator[](const QString &name);
    int maxLen() const;
    int minLen() const;
    void addCell(const QString &name);
    void removeCell(const QString &name);
    void removeCell(int index);
    bool push(int index,const double &val);
    bool push(const QString &name,const double &val);
    bool pop(int index, double &val);
    bool pop(const QString &name, double &val);
    bool isFull() const;
    bool isEmpty() const;
};

class Kebab::Cell {
public:
    typedef double DataType;
private:
    DataType *_cell;
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
    bool push(const double &val);
    bool pop(double &val);
    bool isFull() const;
    bool isEmpty() const;
    int len() const;

private:
    void headForward();
    void tailForward();
};



#endif //REFINE_KEBAB_H
