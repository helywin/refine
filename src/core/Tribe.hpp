//
// Created by jiang.wenqiang on 2018/9/11.
//

#ifndef REFINE_TRIBE_HPP
#define REFINE_TRIBE_HPP

#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QVector>
#include <QtCore/QFile>

class Tribe
{
public:
    class Cell
    {
    public:
        enum DataType
        {
            RawData,
            ProData
        };
    private:
        QString _name;
        int _data_type;
        QVector<float> _data;

    public:
        Cell() : Cell(DataType::RawData) {}

        explicit Cell(DataType type) : _data_type(type) {}

        inline QVector<float> &data() { return _data; }

        inline const QVector<float> &data() const { return _data; }

        inline QString name() const { return _name; }

        inline int dataType() const { return _data_type; }

        inline void setDataType(DataType type) { _data_type = type; }

        inline void push(const float &v) { _data.push_back(v); }

        inline void push(float &&v) { _data.push_back(v); }

        inline float &operator[](int index) { return _data[index]; }

        inline const float &operator[](int index) const { return _data[index]; }

        friend
        QDataStream &operator<<(QDataStream &stream, const Tribe::Cell &cell);

        friend
        QDataStream &operator>>(QDataStream &stream, Tribe::Cell &cell);

        inline int size() const { return _data.size(); }

        inline void resize(int size) { _data.resize(size); }

        inline float last(int index) const
        {
            return _data[_data.size() - 1 - index];
        }
    };

    class Iter
    {
    private:
        Tribe *_tribe;
        int _pos;
    public:
        Iter(Tribe *tribe, int pos) : _tribe(tribe), _pos(pos) {}

        inline bool operator!=(const Iter &other) { return _pos != other._pos; }

        inline Cell &operator*() { return _tribe->_cells[_pos]; }

        inline const Iter operator++()
        {
            _pos += 1;
            return *this;
        }
    };

private:
    QList<Cell> _cells;
    QStringList _header;

public:
    Tribe() = default;

    bool loadFromCsv(QFile &f);

    inline bool loadFromCsv(QFile &&f) { loadFromCsv(f); }

    inline bool loadFromCsv(const QString &f)
    {
        QFile file(f);
        return loadFromCsv(file);
    }

    inline bool loadFromCsv(QString &&f)
    {
        QFile file(f);
        return loadFromCsv(file);
    }

    bool dumpToCsv(QFile &f) const;

    inline bool dumpToCsv(QFile &&f) const { dumpToCsv(f); }

    inline bool dumpToCsv(const QString &f) const
    {
        QFile file(f);
        dumpToCsv(file);
    }

    inline bool dumpToCsv(QString &&f) const
    {
        QFile file(f);
        dumpToCsv(file);
    }

    inline Iter begin() { return Iter(this, 0); }

    inline Iter end() { return Iter(this, _cells.size()); }

    friend QDataStream &operator<<(QDataStream &stream, const Tribe &tribe);

    friend QDataStream &operator>>(QDataStream &stream, Tribe &tribe);

    inline Cell &operator[](int index) { return _cells[index]; }

    inline Cell &operator[](const QString &name)
    {
        return _cells[_header.indexOf(name)];
    }

    inline const Cell &operator[](int index) const { return _cells[index]; }

    inline const Cell &operator[](const QString &name) const
    {
        return _cells[_header.indexOf(name)];
    }

    inline void append(const QString &name, const Cell &cell)
    {
        _cells.append(cell);
        _header.append(name);
    }

    inline void append(const QString &name, Cell &&cell)
    {
        _cells.append(cell);
        _header.append(name);
    }

    inline void append(const QString &name)
    {
        _cells.append(Cell());
        _header.append(name);
    }

    inline void append(const QStringList &names)
    {
        _header.append(names);
        for (int i = 0; i < names.size(); ++i) {
            _cells.append(Cell());
        }
    }

    inline void insert(int index, const QString &name, const Cell &cell)
    {
        _cells.insert(index, cell);
        _header.insert(index, name);
    }

    inline void insert(int index, const QString &name, Cell &&cell)
    {
        _cells.insert(index, cell);
        _header.insert(index, name);
    }

    inline void remove(const QString &name)
    {
        _cells.removeAt(_header.indexOf(name));
        _header.removeOne(name);
    }

    inline void remove(int index)
    {
        _cells.removeAt(index);
        _header.removeAt(index);
    }

    void trim();

    int minLen() const;
    int maxLen() const;

    QStringList zeroLenData() const;

    inline void reset()
    {
        _cells.clear();
        _header.clear();
    }
};

QDataStream &operator<<(QDataStream &stream, const Tribe &tribe);
QDataStream &operator>>(QDataStream &stream, Tribe &tribe);

QDataStream &operator<<(QDataStream &stream, const Tribe::Cell &cell);
QDataStream &operator>>(QDataStream &stream, Tribe::Cell &cell);
#endif //REFINE_TRIBE_HPP
