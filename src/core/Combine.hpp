//
// Created by jiang.wenqiang on 2018/11/28.
//

#ifndef REFINE_COMBINE_HPP
#define REFINE_COMBINE_HPP


#include <QtCore/QVector>
#include "Tribe.hpp"

class Combine
{
public:
    class Cell
    {
    private:
        QVector<float> _data;
    public:
        inline const QVector<float> &data() const { return _data; }

        inline QVector<float> &data() { return _data; }
    };
private:
    QVector<Cell> _cells;
    int _len;

public:
    Combine();

    void reset(int size)
    {
        _cells.clear();
        for (int i = 0; i < size; ++i) {
            _cells.append(Cell());
        }
        _len = 0;
    }

    inline int len() const { return _len; }

    inline int size() const
    {
        if (_cells.isEmpty()) {
            return 0;
        } else {
            return _cells.size();
        }
    }

    inline QVector<Cell> &cells() { return _cells; }

    inline const QVector<Cell> &cells() const { return _cells; }

    Cell &operator[](int index)
    {
        Q_ASSERT(index >= 0 && index < _cells.size());
        return _cells[index];
    }

    const Cell &operator[](int index) const
    {
        Q_ASSERT(index >= 0 && index < _cells.size());
        return _cells[index];
    }

    inline void operator++() { _len += 1; }

    void genFromTribe(const Tribe &tribe);

    inline void append(int index, float y)
    {
        _cells[index].data().append(y);
    }
};


#endif //REFINE_COMBINE_HPP
