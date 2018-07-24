//
// Created by jiang.wenqiang on 2018/6/29.
//

#ifndef CORE_CURVECONFIG_H
#define CORE_CURVECONFIG_H

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QFile>
#include <QtCore/QStringList>


class CurveConfigCell {
public:
    enum Type {
        Physical, Logical
    };

    enum Sample {
        Time, Frame
    };
public:
    unsigned short index;
    QString name;
    QString remark;
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
    Sample sample;
    unsigned long time;
    unsigned long frame;
    long range_in[2];
    long range_out[2];
    QMap<unsigned short, QString> logic;

    CurveConfigCell() = default;

    CurveConfigCell(const CurveConfigCell &cell) = default;

    CurveConfigCell &operator=(CurveConfigCell const &cell) = default;

    bool check();

    QString str();
};


class CurveConfig {
public:
    enum Status {
        Empty, File, Ok, Error
    };
private:
    QStringList config_header;

    QVector<CurveConfigCell> config;

    Status curve_statu;
public:
    CurveConfig();

    bool load(QFile &file);

    void str(QStringList &list);

    const CurveConfigCell &operator[](unsigned int index);

    bool transform(unsigned long id, unsigned char *data,
                   double &result, unsigned short &index);

    Status statu();

    void names(QStringList &list);

    inline int size() const { return config.size(); }
};


#endif //CORE_CURVECONFIG_H
