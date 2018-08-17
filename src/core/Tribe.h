//
// Created by jiang.wenqiang on 2018/7/10.
//

#ifndef REFINE_TRIBE_H
#define REFINE_TRIBE_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QVector>

class Tribe {
public:
    class Cell;

private:
    QVector<QString> _raw_index;
    QList<Cell> _raw_data;
    QVector<QString> _cal_index;
    QList<Cell> _cal_data;

public:
    Tribe() = default;
    int maxLen() const;
    int minLen() const;
    int length() const;
    int rawSize() const;
    int calSize() const;
    int wholeSize() const;
    void addRaw(const QString &name, Cell &&cell);
    void addRaw(const QString &name, const double *data, int len);
    void addCal(const QString &name, Cell &&cell);
    void addCal(const QString &name, const double *data, int len);
    void removeRaw(const QString &name);
    void removeCal(const QString &name);
    Cell &getRaw(const QString &name);
    const Cell &getRaw(const QString &name) const;
    Cell &getCal(const QString &name);
    const Cell &getCal(const QString &name) const;
    int memory() const;
};

class Tribe::Cell {
private:
    QVector<double> _cell;
public:
    Cell() = default;
    Cell(const double *data, int len);
    Cell(const Cell &cell) = default;
    Cell &operator=(const Cell &cell) = default;
    int length() const;
    double *data(int &len);
    const double *data(int &len) const;
};

#endif //CORE_TRIBE_H
