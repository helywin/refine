/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/11
 * @brief 曲线配置
 * @details 曲线配置的读写和其他功能
 ******************************************************************************/

#ifndef REFINE_CURVE_HPP
#define REFINE_CURVE_HPP

#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QFile>
#include <QtCore/QMap>

class Tribe;
class CurveModel;
/*!
 * @brief 曲线配置类
 */

class Curve
{
public:
    friend class CurveModel;
    enum Type
    {
        Physical = 0,
        Logical = 1
    };

    enum Bundle
    {
        None,
        Acceleration,
        EngineSpeed
    };

    class Cell
    {
    public:
        friend class Tribe;
        friend class CurveModel;

    private:
        short _index;
        bool _display;
        QString _name;
        int _type;
        QString _unit;
        short _width;
        unsigned int _color;
        unsigned short _can_id;
        short _zero_byte;
        short _high_byte;
        unsigned char _high_range[2];
        short _low_byte;
        unsigned char _low_range[2];
        int _frame_msec;
        int _range_in[2];
        int _range_out[2];
        QString _remark;
        int _bundle;
        QByteArray _reserved;
    public:
        Cell();
        explicit Cell(int index);
        Cell(int index, Bundle bundle);
        Cell(const Cell &cell) = default;
        friend QDataStream &operator<<(QDataStream &stream, const Cell &cell);
        friend QDataStream &operator>>(QDataStream &stream, Cell &cell);
        Cell &operator=(Cell const &cell) = default;
        bool check() const;
        QStringList str() const;

        //str getter
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

        inline QString canIdStr() const
        {
            return QString("0x%1").arg(_can_id, 3, 16, QChar('0'));
        }

        inline QString zeroByteStr() const
        {
            if (_zero_byte != -1) return QString::number(_zero_byte);
            else return QString("无");
        }

        inline QString highByteStr() const
        {
            if (_high_byte != -1)
                return QString("%1;%2~%3").arg(_high_byte)
                        .arg(_high_range[0]).arg(_high_range[1]);
            else return QString("无");
        }

        inline QString lowByteStr() const
        {
            return QString("%1;%2~%3").arg(_low_byte).arg(_low_range[0])
                    .arg(_low_range[1]);
        }

        inline QString frameMsecStr() const
        {
            return QString::number(_frame_msec);
        }

        inline QString rangeInStr() const
        {
            return QString("%1~%2").arg(_range_in[0]).arg(_range_in[1]);
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

        inline unsigned short canId() const { return _can_id; }

        inline short zeroByte() const { return _zero_byte; }

        inline short highByte() const { return _high_byte; }

        inline const unsigned char *
        highByteRange() const { return _high_range; }

        inline short lowByte() const { return _low_byte; }

        inline const unsigned char *lowByteRange() const { return _low_range; }

        inline int frameMsec() const { return _frame_msec; }

        inline const int *rangeIn() const { return _range_in; }

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

        inline unsigned short &canId() { return _can_id; }

        inline short &zeroByte() { return _zero_byte; }

        inline short &highByte() { return _high_byte; }

        inline unsigned char *highByteRange() { return _high_range; }

        inline short &lowByte() { return _low_byte; }

        inline unsigned char *lowByteRange() { return _low_range; }

        inline int &frameMsec() { return _frame_msec; }

        inline int *rangeIn() { return _range_in; }

        inline int *rangeOut() { return _range_out; }

        inline QString &remark() { return _remark; }

        inline int &bundle() { return _bundle; }

        inline QByteArray &reserved() { return _reserved; }

        //str setter
        inline void setIndexByStr(const QString &s) { _index = s.toUShort(); }

        inline void setDisplayByStr(const QString &s)
        {
            _display = (s == QString("是"));
        }

        inline void setNameByStr(const QString &s) { _name = s; }

        inline void setTypeByStr(const QString &s)
        {
            if (s == QString("物理")) _type = Type::Physical;
            else _type = Type::Logical;
        }

        inline void setUnitByStr(const QString &s) { _unit = s; }

        inline void setWidthByStr(const QString &s) { _width = s.toShort(); }

        inline void setColorByStr(const QString &s)
        {
            _color = s.toULong(nullptr, 16);
        }

        inline void setCanIdByStr(const QString &s)
        {
            _can_id = s.toUShort(nullptr, 16);
        }

        inline void setZeroByteByStr(const QString &s)
        {
            if (s == QString("无")) _zero_byte = -1;
            else { _zero_byte = s.toShort(); }
        }

        void setHighByteByStr(const QString &s);
        void setLowByteByStr(const QString &s);
        void setFrameMsecByStr(const QString &s);

        inline void setRangeInByStr(const QString &s)
        {
            QStringList list = s.split(QChar('~'));
            _range_in[0] = list[0].toInt();
            _range_in[1] = list[1].toInt();
        }

        inline void setRangeOutByStr(const QString &s)
        {
            QStringList list = s.split(QChar('~'));
            _range_out[0] = list[0].toInt();
            _range_out[1] = list[1].toInt();
        }

        inline void setRemarkByStr(const QString &s) { _remark = s; }

        inline void setBundleByStr(const QString &s)
        {
            _bundle = (Bundle) s.toInt();
        }

    };

    class Iter
    {
    private:
        Curve *_curve;
        int _pos;

    public:
        Iter(Curve *curve, int pos) : _curve(curve), _pos(pos) {}

        inline bool operator!=(const Iter &other) { return _pos != other._pos; }

        inline Cell &operator*() { return (*_curve)[_pos]; }

        const Iter &operator++()
        {
            _pos += 1;
            return *this;
        };
    };

    class IterConst
    {
    private:
        const Curve *_curve;
        int _pos;

    public:
        IterConst(const Curve *curve, int pos) : _curve(curve), _pos(pos) {}

        inline bool operator!=(const IterConst &other)
        {
            return _pos != other._pos;
        }

        inline const Cell &operator*() { return (*_curve)[_pos]; }

        const IterConst &operator++()
        {
            _pos += 1;
            return *this;
        };
    };

    typedef QMap<int, QList<int>> SubIdMap;
    typedef QMap<int, QList<int>> OtherIdMap;

private:
    QStringList _header;
    QList<Cell> _cells;
    SubIdMap _sub_id_map_777;      //用于曲线转换查表而非遍历
    OtherIdMap _other_id_map;
    bool _initialized;

public:
    Curve() : _initialized(false) {}

    bool loadFromCsv(QFile &f);

    inline bool loadFromCsv(QFile &&f) { return loadFromCsv(f); }

    inline bool loadFromCsv(const QString &f)
    {
        QFile file(f);
        return loadFromCsv(file);
    }

    inline bool loadFromCsv(QString &&f) { return loadFromCsv(f); }

    bool dumpToCsv(QFile &f) const;

    inline bool dumpToCsv(QFile &&f) const { return dumpToCsv(f); }

    inline bool dumpToCsv(const QString &f) const
    {
        QFile file(f);
        return dumpToCsv(file);
    }

    inline bool dumpToCsv(QString &&f) const { return dumpToCsv(f); }

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

//    friend QDataStream;
    friend QDataStream &operator<<(QDataStream &stream, const Curve &curve);
    friend QDataStream &operator>>(QDataStream &stream, Curve &curve);

    inline Cell &at(int index) { return (*this)[index]; }

    inline Cell &at(const QString &name) { return (*this)[name]; }

    inline const Cell &at(int index) const { return (*this)[index]; }

    inline const Cell &at(const QString &name) const { return (*this)[name]; }

    inline const QStringList &header() const { return _header; }

    inline int size() const { return _cells.size(); }

    QStringList str() const;
    void append(const Cell &cell);
    void append(Cell &&cell);
    void insert(int index, const Curve::Cell &cell);
    void insert(Cell &&cell, int index);
    void remove(int index);
    void remove(const QString &name);
    void clear();

    inline Iter begin() { return Iter(this, 0); }

    inline IterConst begin() const { return IterConst(this, 0); }

    inline Iter end() { return Iter(this, _cells.size()); }

    inline IterConst end() const { return IterConst(this, _cells.size()); }

    inline void setInitialized(bool flag) { _initialized = flag; }

    inline bool isInitialized() const { return _initialized; }

    void updateHeader();
};

QDataStream &operator<<(QDataStream &stream, const Curve &curve);
QDataStream &operator>>(QDataStream &stream, Curve &curve);
QDataStream &operator<<(QDataStream &stream, const Curve::Cell &cell);
QDataStream &operator>>(QDataStream &stream, Curve::Cell &cell);


#endif //REFINE_CURVE_HPP
