//
// Created by jiang.wenqiang on 2018/7/10.
//

#ifndef REFINE_TRIBE_H
#define REFINE_TRIBE_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QVector>
#include "Define.h"

class Tribe {
public:
    class Cell;

    enum class DataType {
        Raw = 0,
        Pro = 1
    };

private:
    int _length;
    QStringList _raw_index;
    QList<Cell> _raw_data;
    QStringList _pro_index;
    QList<Cell> _pro_data;

public:
    Tribe() = delete;

    explicit Tribe(int len);

    int length() const;

    int rawSize() const;

    int proSize() const;

    int wholeSize() const;

    void addEmptyCurves(const QStringList &index, DataType type);

    void addEmptyCurve(const QString &name, DataType type);

    void addCurve(const QStringList &name, const Cell &&cell, DataType type);

    void addCurve(const QString &name, const float_u *data,
                  Tribe::DataType type);

    void removeCurve(const QString &name, DataType type);

    Cell &raw(const QString &name);

    const Cell &raw(const QString &name) const;

    Cell &pro(const QString &name);

    const Cell &pro(const QString &name) const;

    QStringList rawIndex() const;

    QStringList proIndex() const;

    int memory() const;

};

class Tribe::Cell {
private:
    QVector<float_u> _cell;
public:
    Cell() = default;

    Cell(const float_u *data, int len);

    explicit Cell(int len);

    Cell(const Cell &cell) = default;

    Cell &operator=(const Cell &cell) = default;

    float_u &operator[](int index);

    const float_u &operator[](int index) const;

    int length() const;

    float_u *data(int &len);

    const float_u *data(int &len) const;

    float_u *data();

    const float_u *data() const;

    void append(float_u &v);

    void append(float_u &&v);
};

#endif //CORE_TRIBE_H
