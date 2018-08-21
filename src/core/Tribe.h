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

    enum class DataType {
        Raw = 0,
        Calculated = 1
    };

private:
    QStringList _raw_index;
    QList<Cell> _raw_data;
    QStringList _cal_index;
    QList<Cell> _cal_data;

public:
    Tribe() = default;

    int maxLen() const;

    int minLen() const;

    int length() const;

    int rawSize() const;

    int calSize() const;

    int wholeSize() const;

    bool isAligned() const;

    void setIndex(const QStringList &index, DataType type);

    void addSequence(const QStringList &v, DataType type);

    void addWholeCurve(const QStringList &name, Cell &&cell, DataType type);

    void addWholeCurve(const QString &name, const double *data, int len,
                       DataType type);

    void removeWholeCurve(const QString &name, DataType type);

    Cell &raw(const QString &name);

    const Cell &raw(const QString &name) const;

    Cell &cal(const QString &name);

    const Cell &cal(const QString &name) const;

    QStringList rawIndex() const;

    QStringList calIndex() const;

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

    double &operator[](int index);

    const double &operator[](int index) const;

    int length() const;

    double *data(int &len);

    const double *data(int &len) const;

    void append(double &v);

    void append(double &&v);
};

#endif //CORE_TRIBE_H
