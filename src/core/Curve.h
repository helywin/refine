//
// Created by jiang.wenqiang on 2018/6/29.
//

#ifndef CORE_CURVE_H
#define CORE_CURVE_H

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QFile>
#include <QtCore/QStringList>

class Transform;

class Curve {
public:

    class Cell;

    enum Status {
        Empty, File, Ok, Error
    };
    friend class Transform;
private:
    QStringList _header;

    QVector<Cell> _config;

    Status _status;
public:
    Curve();

    bool loadCsv(QFile &csv);

    void str(QStringList &list);

    const Cell &operator[](unsigned int index);

    bool transform(unsigned long id, unsigned char *data,
                   double &result, unsigned short &index);

    Status status() const;

    void names(QStringList &list);

    inline int size() const { return _config.size(); }
};

class Curve::Cell {
public:
    enum Type {
        Physical, Logical
    };

    enum Sample {
        Time, Frame
    };
public:
    unsigned short index;
    bool display;
    QString name;
    Type type;
    QString unit;
    unsigned short width;
    unsigned long color;
    unsigned long can_id;
    bool zero_flag;
    unsigned short zero_byte;
    bool high_flag;
    unsigned short high_byte;   //添加上去
    unsigned short high_range[2];
    unsigned short low_byte;
    unsigned short low_range[2];
    Sample sample_type;
    unsigned long sample;
    long range_in[2];
    long range_out[2];
    QMap<unsigned short, QString> logic;
    QString remark;

    Cell() = default;

    Cell(const Cell &cell) = default;

    Cell &operator=(Cell const &cell) = default;

    bool check();

    QString str();
};

#endif //CORE_CURVE_H
