//
// Created by jiang.wenqiang on 2018/9/11.
//

#ifndef REFINE_CURVE_H
#define REFINE_CURVE_H

#include <QtCore/QStringList>
#include <QtCore/QVector>
#include <QtCore/QPoint>
#include <QtCore/QMap>
#include <QtCore/QFile>

class Curve
{
public:
    class Cell;
    class Iter;

    typedef QStringList Header;

    enum Status
    {
        Uninitialized,
        Initialized
    };

    enum Bundle
    {
        None,
        Acceleration,
        EngineSpeed,

    };

private:
    QStringList _table_head;
    Status _status;
    Header _header;
    QVector<Cell> _cells;

public:
    Curve();
    bool loadFromCsv(QFile &f);
    inline bool loadFromCsv(QFile &&f) { return loadFromCsv(f); }
    inline bool loadFromCsv(QString &f)
    { QFile file(f); return loadFromCsv(file); }
    inline bool loadFromCsv(QString &&f) { return loadFromCsv(f); }
    bool dumpToCsv(QFile &f) const;
    inline bool dumpToCsv(QFile &&f) const { return dumpToCsv(f); }
    inline bool dumpToCsv(QString &f) const
    { QFile file(f); return dumpToCsv(file); }
    inline bool dumpToCsv(QString &&f) const { return dumpToCsv(f); }
    inline Cell &operator[](int index) { return _cells[index]; }
    inline Cell &operator[](const QString &name)
    { return _cells[_header.indexOf(name)]; }
    inline const Cell &operator[](int index) const { return _cells[index]; }
    inline const Cell &operator[](const QString &name) const
    { return _cells[_header.indexOf(name)]; }
//    friend QDataStream;
    friend QDataStream &operator<<(QDataStream &stream, const Curve &curve);
    friend QDataStream &operator>>(QDataStream &stream, Curve &curve);
    inline Cell &at(int index) { return (*this)[index]; }
    inline Cell &at(const QString &name) { return (*this)[name]; }
    inline const Cell &at(int index) const { return (*this)[index]; }
    inline const Cell &at(const QString &name) const { return (*this)[name]; }
    inline QStringList header() const { return _header; }
    inline int size() const { return _cells.size(); }
    QStringList str() const;
    void append(const Cell &cell);
    void append(Cell &&cell);
    void appendRow();
    void appendRow(Bundle bundle);
    void insertRow(int index);
    void insertRow(int index, Bundle bundle);
    void removeRow(int index);
    inline void removeAllRow() { _cells.clear(); _header.clear(); }
    void clear();
};

QDataStream &operator<<(QDataStream &stream, const Curve &curve);
QDataStream &operator>>(QDataStream &stream, Curve &curve);

#define CURVE_NAME_L 32
#define CURVE_UNIT_L 16
#define CURVE_REMARK_L 64

class Curve::Cell
{
public:
    enum Type
    {
        Physical = 0,
        Logical = 1
    };

    enum Sample
    {
        Timed = 0,
        Framed = 1
    };

    typedef QMap<unsigned int, QString> Logic;

private:
    unsigned short _index;
    bool _display;
    QString _name;
    Type _type;
    QString _unit;
    unsigned short _width;
    unsigned long _color;
    unsigned long _can_id;
    bool _zero_byte_existed;
    unsigned short _zero_byte;
    bool _high_byte_existed;
    unsigned short _high_byte;
    unsigned short _high_byte_range[2];
    unsigned short _low_byte;
    unsigned short _low_byte_range[2];
    Sample _sample_type;
    unsigned long _sample;
    long _range_in[2];
    long _range_out[2];
    Logic _logic_map;
    QString _remark;

    Bundle _bundle;

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

    inline QString displayStr() const { return QString::number(_display); }

    inline QString nameStr() const { return QString(_name); }

    inline QString typeStr() const
    { if (_type == Type::Logical) return QString("逻辑");
      else return QString("物理"); }

    inline QString unitStr() const { return QString(_unit); }

    inline QString widthStr() const { return QString::number(_width); }

    inline QString colorStr() const
    { return QString("0x%1").arg(_color, 6, 16, QChar('0')); }

    inline QString canIdStr() const
    { return QString("0x%1").arg(_can_id, 3, 16, QChar('0')); }

    inline QString zeroByteStr() const
    { if (_zero_byte_existed) return QString("%1").arg(_zero_byte);
      else return QString("无"); }

    inline QString highByteStr() const
    { if (_high_byte_existed) return QString("%1;%2~%3").arg(_high_byte)
                    .arg(_high_byte_range[0]).arg(_high_byte_range[1]);
      else return QString("无"); }
    inline QString lowByteStr() const
    { return QString("%1;%2~%3").arg(_low_byte).arg(_low_byte_range[0])
    .arg(_low_byte_range[1]); }

    inline QString sampleStr() const
    { if (_sample_type == Sample::Framed ) return QString("帧数;%1")
    .arg(_sample);
      else  return QString("时间;%1").arg(_sample); }

    inline QString rangeInStr() const
    { return QString("%1~%2").arg(_range_in[0]).arg(_range_in[1]); }

    inline QString rangeOutStr() const
    { return QString("%1~%2").arg(_range_out[0]).arg(_range_out[1]); }

    QString logicMapStr() const {  }

    inline QString remarkStr() const { return QString(_remark); }

    //val getter
    inline unsigned short index() const { return _index; }
    inline bool display() const { return _display; }
    inline QString name() const { return _name; }
    inline Type type() const { return _type; }
    inline QString unit() const { return _unit; }
    inline unsigned short width() const { return _width; }
    inline unsigned long color() const { return _color; }
    inline unsigned long canId() const { return _can_id; }
    inline bool zeroByteExisted() const { return _zero_byte_existed; }
    inline unsigned short zeroByte() const { return _zero_byte; }
    inline bool highByteExisted() const { return _high_byte_existed; }
    inline unsigned short highByte() const { return _high_byte; }
    inline const unsigned short *highByteRange() const
    { return _high_byte_range; }
    inline unsigned short lowByte() const { return _low_byte; }
    inline const unsigned short *lowByteRange() const
    { return _low_byte_range; }
    inline Sample sampleType() const { return _sample_type; }
    inline unsigned long sample() const { return _sample; }
    inline const long *rangeIn() const { return _range_in; }
    inline const long *rangeOut() const { return _range_out; }
    inline QMap<unsigned int, QString> logicMap() const { return _logic_map; }
    inline QString remark() const { return _remark; }

    //str setter
    inline void setIndexByStr(QString &s) { _index = s.toUShort(); }
    inline void setDisplayByStr(QString &s) { _display = s == QString("是"); }
    inline void setNameByStr(QString &s) { _name = s; }
    inline void setTypeByStr(QString &s)
    { if (s == QString("物理"))  _type = Type::Physical;
      else _type = Type::Logical; }

    inline void setUnitByStr(QString &s) { _unit = s; }
    inline void setWidthByStr(QString &s) { _width = s.toUShort(); }
    inline void setColorByStr(QString &s) { _color = s.toULong(nullptr, 16); }
    inline void setCanIdByStr(QString &s) { _can_id = s.toULong(nullptr, 16); }
    inline void setZeroByteByStr(QString &s)
    { if (s == QString("无")) _zero_byte_existed = false;
      else { _zero_byte_existed = true; _zero_byte = s.toUShort(); } }

    void setHighByteByStr(QString &s);
    void setLowByteByStr(QString &s);
    void setSampleByStr(QString &s);

    inline void setRangeInByStr(QString &s)
    { QStringList list = s.split(QChar('~'));
        _range_in[0] = list[0].toLong();
        _range_in[1] = list[1].toLong(); }

    inline void setRangeOutByStr(QString &s)
    { QStringList list = s.split(QChar('~'));
        _range_out[0] = list[0].toLong();
        _range_out[1] = list[1].toLong(); }

    inline void setLogicMapByStr(QString &s) {  }
    inline void setRemarkByStr(QString &s) { _remark = s; }

    //val setter
    inline void setIndexByVal(unsigned short v) { _index = v; }
    inline void setDisplayByVal(bool v) { _display = v; }
    inline void setNameByVal(const QString &v) { _name = v; }
    inline void setNameByVal(QString &&v) { _name = v; }
    inline void setTypeByVal(Type v) { _type = v; }
    inline void setUnitByVal(const QString &v) { _unit = v; }
    inline void setUnitByVal(QString &&v) { _unit = v; }
    inline void setWidthByVal(unsigned short v) { _width = v; }
    inline void setColorByVal(unsigned long v) { _color = v; }
    inline void setCanIdByVal(unsigned long v) { _can_id = v; }
    inline void setZeroByteExistedByVal(bool v) { _zero_byte_existed = v; }
    inline void setZeroByteByVal(unsigned short v) { _zero_byte = v; }
    inline void setHighByteExistedByVal(bool v) { _high_byte_existed = v; }
    inline void setHighByteByVal(unsigned short v) { _high_byte = v; }
    inline void setHighByteRangeByVal(const unsigned short *v)
    { _high_byte_range[0] = v[0]; _high_byte_range[1] = v[1]; }
    inline void setHighByteRangeByVal(unsigned short v0, unsigned short v1)
    { _high_byte_range[0] = v0; _high_byte_range[1] = v1; }
    inline void setLowByteByVal(unsigned short v) { _low_byte = v; }
    inline void setLowByteRangeByVal(const unsigned short *v)
    { _low_byte_range[0] = v[0]; _low_byte_range[1] = v[1]; }
    inline void setLowByteRangeByVal(unsigned short v0, unsigned short v1)
    { _low_byte_range[0] = v0; _low_byte_range[1] = v1; }
    inline void setSampleTypeByVal(Sample v) { _sample_type = v; }
    inline void setSampleByVal(unsigned long v) { _sample = v; }
    inline void setRangeInByVal(const long *v)
    { _range_in[0] = v[0]; _range_in[1] = v[1]; }
    inline void setRangeInByVal(long v0, long v1)
    { _range_in[0] = v0; _range_in[1] = v1; }
    inline void setRangeOutByVal(const long *v)
    { _range_out[0] = v[0]; _range_out[1] = v[1]; }
    inline void setRangeOutByVal(long v0, long v1)
    { _range_out[0] = v0; _range_out[1] = v1; }
    inline void setLogicMapByVal(const Logic &v) { _logic_map = v; }
    inline void setRemarkByVal(const QString &v) { _remark = v; }
    inline void setRemarkByVal(QString &&v) { _remark = v; }
};

QDataStream &operator<<(QDataStream &stream, const Curve::Cell &cell);
QDataStream &operator>>(QDataStream &stream, Curve::Cell &cell);

class Curve::Iter {
private:
    Curve *_curve;
    int _pos;

public:
    Iter(Curve *curve, int pos);
    bool operator!=(const Iter &other);
    Cell &operator*();
    const Iter &operator++();
};

#endif //REFINE_CURVE_H
