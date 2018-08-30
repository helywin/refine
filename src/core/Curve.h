//
// Created by jiang.wenqiang on 2018/8/6.
//

#ifndef REFINE_CURVE_H
#define REFINE_CURVE_H

#include <QtCore/QStringList>
#include <QtCore/QVector>
#include <QtCore/QMap>
#include <QtCore/QFile>

class Curve {
public:
    class Cell;

    typedef QStringList Header;

    enum class Status {
        Uninitialized,
        Initialized
    };

    enum class FileType {
        Csv,
        Ref,
    };

    enum class Bundle {
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

    bool load(QFile &f, FileType type);

    bool dump(QFile &f, FileType type) const;

    Cell &operator[](int index);

    const Cell &operator[](int index) const;

    Cell &operator[](const QString &name);

    const Cell &operator[](const QString &name) const;

    Cell &at(int index);

    const Cell &at(int index) const;

    Cell &at(const QString &name);

    const Cell &at(const QString &name) const;

    QStringList header() const;

    int size() const;

    QStringList str() const;

    void appendRow();

    void appendRow(Bundle bundle);

    void insertRow(int index);

    void insertRow(int index, Bundle bundle);

    void removeRow(int index);

    void removeAllRow();

    void clear();

private:
    bool loadFromCsv(QFile &f);

    bool loadFromRef(QFile &f);

    bool dumpToCsv(QFile &f) const;

    bool dumpToRef(QFile &f) const;
};


class Curve::Cell {
public:
    enum Type {
        Physical,
        Logical
    };

    enum Sample {
        Timed,
        Framed
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

    Cell &operator=(Cell const &cell) = default;

    bool check() const;

    QStringList str() const;

    //str getter

    QString indexStr() const;

    QString displayStr() const;

    QString nameStr() const;

    QString typeStr() const;

    QString unitStr() const;

    QString widthStr() const;

    QString colorStr() const;

    QString canIdStr() const;

    QString zeroByteStr() const;

    QString highByteStr() const;

    QString lowByteStr() const;

    QString sampleStr() const;

    QString rangeInStr() const;

    QString rangeOutStr() const;

    QString logicMapStr() const;

    QString remarkStr() const;

    //val getter

    unsigned short index() const;

    bool display() const;

    QString name() const;

    Type type() const;

    QString unit() const;

    unsigned short width() const;

    unsigned long color() const;

    unsigned long canId() const;

    bool zeroByteExisted() const;

    unsigned short zeroByte() const;

    bool highByteExisted() const;

    unsigned short highByte() const;

    const unsigned short *highByteRange() const;

    unsigned short lowByte() const;

    const unsigned short *lowByteRange() const;

    Sample sampleType() const;

    unsigned long sample() const;

    const long *rangeIn() const;

    const long *rangeOut() const;

    QMap<unsigned int, QString> logicMap() const;

    QString remark() const;

    //str setter

    void setIndexByStr(QString &s);

    void setDisplayByStr(QString &s);

    void setNameByStr(QString &s);

    void setTypeByStr(QString &s);

    void setUnitByStr(QString &s);

    void setWidthByStr(QString &s);

    void setColorByStr(QString &s);

    void setCanIdByStr(QString &s);

    void setZeroByteByStr(QString &s);

    void setHighByteByStr(QString &s);

    void setLowByteByStr(QString &s);

    void setSampleByStr(QString &s);

    void setRangeInByStr(QString &s);

    void setRangeOutByStr(QString &s);

    void setLogicMapByStr(QString &s);

    void setRemarkByStr(QString &s);

    //val setter

    void setIndexByVal(unsigned short v);

    void setDisplayByVal(bool v);

    void setNameByVal(const QString &v);

    void setTypeByVal(Type v);

    void setUnitByVal(const QString &v);

    void setWidthByVal(unsigned short v);

    void setColorByVal(unsigned long v);

    void setCanIdByVal(unsigned long v);

    void setZeroByteExistedByVal(bool v);

    void setZeroByteByVal(unsigned short v);

    void setHighByteExistedByVal(bool v);

    void setHighByteByVal(unsigned short v);

    void setHighByteRangeByVal(const unsigned short *v);

    void setLowByteByVal(unsigned short v);

    void setLowByteRangeByVal(const unsigned short *v);

    void setSampleTypeByVal(Sample v);

    void setSampleByVal(unsigned long v);

    void setRangeInByVal(const long *v);

    void setRangeOutByVal(const long *v);

    void setLogicMapByVal(const Logic &v);

    void setRemarkByVal(const QString &v);
};


#endif //REFINE_CURVE_H
