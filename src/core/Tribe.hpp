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

#include "Curve.hpp"


/*!
 * @brief 存储数据的缓冲区
 */

class Tribe
{
public:
    friend class Curve;

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

    class Style
    {
    public:
        enum Type
        {
            Physical = 0,
            Logical = 1
        };

        enum Bundle
        {
            None = 0,
            Acceleration,
            EngineSpeed
        };
    private:
        short _index;
        bool _display;  //
        QString _name;  //
        int _type;
        QString _unit;  //
        short _width;   //
        unsigned int _color;    //
        int _range_out[2];      //
        QString _remark;        //
        int _bundle;            //
        QByteArray _reserved;
    public:
        explicit Style(const Curve::Cell &cell);
        Style(const Style &style) = default;
        friend QDataStream &operator<<(QDataStream &stream, const Style &style);
        friend QDataStream &operator>>(QDataStream &stream, Style &style);
        Style &operator=(const Style &cell) = default;
        Style &operator=(const Curve::Cell &cell);

        inline QString indexStr() const { return QString::number(_index); }

        inline QString displayStr() const
        {
            if (_display) {
                return QString("是");
            } else {
                return QString("否");
            }
        }

        inline QString nameStr() const { return QString(_name); }

        inline QString typeStr() const
        {
            if (_type == Type::Logical) return QString("逻辑");
            else return QString("物理");
        }

        inline QString unitStr() const { return QString(_unit); }

        inline QString widthStr() const { return QString::number(_width); }

        inline QString colorStr() const
        {
            return QString("0x%1").arg(_color, 6, 16, QChar('0'));
        }

        inline QString rangeOutStr() const
        {
            return QString("%1~%2").arg(_range_out[0]).arg(_range_out[1]);
        }

        inline QString remarkStr() const { return QString(_remark); }

        inline QString bundleStr() const { return QString::number(_bundle); }

        //val getter
        inline short index() const { return _index; }

        inline bool display() const { return _display; }

        inline QString name() const { return _name; }

        inline int type() const { return _type; }

        inline QString unit() const { return _unit; }

        inline short width() const { return _width; }

        inline unsigned int color() const { return _color; }

        inline const int *rangeOut() const { return _range_out; }

        inline QString remark() const { return _remark; }

        inline int bundle() const { return _bundle; }

        inline QByteArray reserved() const { return _reserved; }

        //getter

        inline short &index() { return _index; }

        inline bool &display() { return _display; }

        inline QString &name() { return _name; }

        inline int &type() { return _type; }

        inline QString &unit() { return _unit; }

        inline short &width() { return _width; }

        inline unsigned int &color() { return _color; }

        inline int *rangeOut() { return _range_out; }

        inline QString &remark() { return _remark; }

        inline int &bundle() { return _bundle; }

        inline QByteArray &reserved() { return _reserved; }


        //str setter
        inline void setIndexByStr(QString &s) { _index = s.toUShort(); }

        inline void setDisplayByStr(QString &s)
        {
            _display = (s == QString("是"));
        }

        inline void setNameByStr(QString &s) { _name = s; }

        inline void setTypeByStr(QString &s)
        {
            if (s == QString("物理")) _type = Type::Physical;
            else _type = Type::Logical;
        }

        inline void setUnitByStr(QString &s) { _unit = s; }

        inline void setWidthByStr(QString &s) { _width = s.toShort(); }

        inline void setRangeOutByStr(QString &s)
        {
            QStringList list = s.split(QChar('~'));
            _range_out[0] = list[0].toInt();
            _range_out[1] = list[1].toInt();
        }

        inline void setRemarkByStr(QString &s) { _remark = s; }

        inline void setBundleByStr(QString &s) { _bundle = (Bundle) s.toInt(); }
    };

    class Cell
    {
    private:
        friend class Tribe;
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

        inline float &operator[](int index)
        {
            Q_ASSERT(index < _data.size() && index >= 0);
            return _data[index];
        }

        inline const float &operator[](int index) const
        {
            Q_ASSERT(index < _data.size() && index >= 0);
            return _data[index];
        }

        friend
        QDataStream &operator<<(QDataStream &stream, const Tribe::Cell &cell);

        friend
        QDataStream &operator>>(QDataStream &stream, Tribe::Cell &cell);

        inline int size() const { return _data.size(); }

        inline void resize(int size) { _data.resize(size); }

        //! \brief 清空数据
        void reset();

        inline bool fill() const { return _fill_this; }

        inline void setFill(Fill fill) { _fill_this = fill; }

        float fakePercent() const;

        inline QString fakePercentStr() const
        {
            return QString("%1%").arg(fakePercent() * 100);
        }

        inline bool empty() const { return _empty; }
    };

    /*!
     * @brief 迭代器
     */
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
    QList<Style> _styles;       //! \brief 曲线显示配置
    QList<Cell> _cells;         //! \brief 曲线数据
    QStringList _header;        //! \brief 曲线名字表
    QVector<int> _segment;      //! \brief 曲线分段
    int _len;

public:
    Tribe();

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

    inline int len() const { return _len; }

    inline int size() const { return _cells.size(); }

    inline void clear()
    {
        _styles.clear();
        _cells.clear();
        _header.clear();
        _segment.clear();
    }

    void reset();

    void setUnFilled();

    inline void newSegment() { _segment.append(_cells[0].size()); }

    void genFromCurve(const Curve &curve);

    inline void operator++() { _len += 1; }

    inline QList<Style> &styles() { return _styles; }

    inline const QList<Style> &styles() const { return _styles; }

    inline Style &style(int index) { return _styles[index]; }

    inline const Style &style(int index) const { return _styles[index]; }
};

QDataStream &operator<<(QDataStream &stream, const Tribe &tribe);
QDataStream &operator>>(QDataStream &stream, Tribe &tribe);

QDataStream &operator<<(QDataStream &stream, const Tribe::Style &style);
QDataStream &operator>>(QDataStream &stream, Tribe::Style &style);

QDataStream &operator<<(QDataStream &stream, const Tribe::Cell &cell);
QDataStream &operator>>(QDataStream &stream, Tribe::Cell &cell);


#endif //REFINE_TRIBE_HPP
