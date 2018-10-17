/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/11
 * @brief 曲线数据缓冲区
 * @details 存储曲线数据缓冲区
 ******************************************************************************/

#ifndef REFINE_TRIBE_HPP
#define REFINE_TRIBE_HPP

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVector>
#include <QtCore/QFile>

/*!
 * @brief 存储数据的缓冲区
 */

class Tribe
{
public:
    enum DataType
    {
        RawData,
        ProData
    };

    enum Fill : char
    {
        Data = 0,
        FakeByPrevious = 1,
        FakeByZero = 2,
    };

    class Cell
    {
    private:
        QString _name;
        int _data_type;
        QVector<float> _data;
        bool _fill_this;
        QVector<Fill> _fill;
        bool _empty;            //方便图像绘制

    public:
        explicit Cell(const QString &name, DataType type = RawData) :
                _name(name), _data_type(type), _data(), _fill_this(false),
                _empty(true) {}

        explicit Cell(QString &&name, DataType type = RawData) :
                Cell(name, type) {}

//        inline QVector<float> &data() { return _data; }

        inline const QVector<float> &data() const { return _data; }

        inline QString name() const { return _name; }

        inline int dataType() const { return _data_type; }

        inline void setDataType(DataType type) { _data_type = type; }

        void push(Fill fill, const float &v);

        inline void push(Fill fill, float &&v) { push(fill, v); }

        inline float &operator[](int index) { return _data[index]; }

        inline const float &operator[](int index) const { return _data[index]; }

        friend
        QDataStream &operator<<(QDataStream &stream, const Tribe::Cell &cell);

        friend
        QDataStream &operator>>(QDataStream &stream, Tribe::Cell &cell);

        inline int size() const { return _data.size(); }

        inline void resize(int size) { _data.resize(size); }

        inline void reset() { _data.clear(); }

        inline bool fill() const { return _fill_this; }

        inline void setFill(Fill fill) { _fill_this = fill; }

        float fakePercent() const;

        inline QString fakePercentStr() const
        {
            return QString("%1%").arg(fakePercent() * 100);
        }

        inline bool empty() const { return _empty; }
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

    inline bool loadFromCsv(QFile &&f) { return loadFromCsv(f); }

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

    inline bool dumpToCsv(QFile &&f) const { return dumpToCsv(f); }

    inline bool dumpToCsv(const QString &f) const
    {
        QFile file(f);
        return dumpToCsv(file);
    }

    inline bool dumpToCsv(QString &&f) const
    {
        QFile file(f);
        return dumpToCsv(file);
    }

    inline Iter begin() { return Iter(this, 0); }

    inline Iter end() { return Iter(this, _cells.size()); }

    friend QDataStream &operator<<(QDataStream &stream, const Tribe &tribe);

    friend QDataStream &operator>>(QDataStream &stream, Tribe &tribe);

    inline Cell &operator[](int index)
    {
        Q_ASSERT(index >= 0 && index < _cells.size());
        return _cells[index];
    }

    inline Cell &operator[](const QString &name)
    {
        Q_ASSERT(_header.contains(name));
        return _cells[_header.indexOf(name)];
    }

    inline const Cell &operator[](int index) const
    {
        Q_ASSERT(index >= 0 && index < _cells.size());
        return _cells[index];
    }

    inline const Cell &operator[](const QString &name) const
    {
        Q_ASSERT(_header.contains(name));
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
        _cells.append(Cell(name));
        _header.append(name);
    }

    inline void append(const QStringList &names)
    {
        _header.append(names);
        for (int i = 0; i < names.size(); ++i) {
            _cells.append(Cell(names[i]));
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

    inline void clear()
    {
        _cells.clear();
        _header.clear();
    }

    void reset();

    void addGap();

    void setUnFilled();
};

QDataStream &operator<<(QDataStream &stream, const Tribe &tribe);
QDataStream &operator>>(QDataStream &stream, Tribe &tribe);

QDataStream &operator<<(QDataStream &stream, const Tribe::Cell &cell);
QDataStream &operator>>(QDataStream &stream, Tribe::Cell &cell);
#endif //REFINE_TRIBE_HPP
