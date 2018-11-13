/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/4
 * @brief Softcan配置读取
 * @details 实现读取Softcan配置并转换为Curve配置
 ******************************************************************************/

#ifndef CORE_SOFTCAN_H
#define CORE_SOFTCAN_H

#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QString>
#include <QtCore/QDebug>
#include "Curve.hpp"
#include "Message.hpp"

/*!
 * @brief 把Softcan的曲线配置转化为可用配置的类
 */

class Softcan : public Message
{
public:
    class Cell
    {
    private:
        int _index;
        int _can_id;
        int _zero_byte;
        int _high_byte;
        int _low_byte;
        int _first_bit;
        int _second_bit;
        int _third_bit;
        int _input_max;
        int _input_min;
        unsigned int _color;
        int _width;
        bool _visible;
        int _sample_mode;
        int _sample_num;
        int _style;
        bool _enable_update;
        int _size_x;
        int _size_y;
        double _x_max;
        double _x_min;
        double _y_max;
        double _y_min;
        double _x_temp;
        double _y_temp;
        QString _analog_mean;
        QString _unit;
        QString _intro;
        QString _name;
        bool _save_flag;

    public:
        Cell();

        friend QDataStream &operator>>(QDataStream &stream, Softcan::Cell &cell);
        static QString readWString(QDataStream &stream);

        inline int index() const { return _index; }

        inline int &index() { return _index; }

        inline int canId() const { return _can_id; }

        inline int &canId() { return _can_id; }

        inline int zeroByte() const { return _zero_byte; }

        inline int &zeroByte() { return _zero_byte; }

        inline int highByte() const { return _high_byte; }

        inline int &highByte() { return _high_byte; }

        inline int lowByte() const { return _low_byte; }

        inline int &lowByte() { return _low_byte; }

        inline int firstBit() const { return _first_bit; }

        inline int &firstBit() { return _first_bit; }

        inline int secondBit() const { return _second_bit; }

        inline int &secondBit() { return _second_bit; }

        inline int thirdBit() const { return _third_bit; }

        inline int &thirdBit() { return _third_bit; }

        inline int inputMax() const { return _input_max; }

        inline int &inputMax() { return _input_max; }

        inline int inputMin() const { return _input_min; }

        inline int &inputMin() { return _input_min; }

        inline unsigned int color() const { return _color; }

        inline unsigned int &color() { return _color; }

        inline int width() const { return _width; }

        inline int &width() { return _width; }

        inline bool visible() const { return _visible; }

        inline bool &visible() { return _visible; }

        inline int sampleMode() const { return _sample_mode; }

        inline int &sampleMode() { return _sample_mode; }

        inline int sampleNum() const { return _sample_num; }

        inline int &sampleNum() { return _sample_num; }

        inline int style() const { return _style; }

        inline int &style() { return _style; }

        inline bool enableUpdate() const { return _enable_update; }

        inline bool &enableUpdate() { return _enable_update; }

        inline int sizeX() const { return _size_x; }

        inline int &sizeX() { return _size_x; }

        inline int sizeY() const { return _size_y; }

        inline int &sizeY() { return _size_y; }

        inline double xMax() const { return _x_max; }

        inline double &xMax() { return _x_max; }

        inline double xMin() const { return _x_min; }

        inline double &xMin() { return _x_min; }

        inline double yMax() const { return _y_max; }

        inline double &yMax() { return _y_max; }

        inline double yMin() const { return _y_min; }

        inline double &yMin() { return _y_min; }

        inline double xTemp() const { return _x_temp; }

        inline double &xTemp() { return _x_temp; }

        inline double yTemp() const { return _y_temp; }

        inline double &yTemp() { return _y_temp; }

        inline QString analogMean() const { return _analog_mean; }

        inline QString &analogMean() { return _analog_mean; }

        inline QString unit() const { return _unit; }

        inline QString &unit() { return _unit; }

        inline QString intro() const { return _intro; }

        inline QString &intro() { return _intro; }

        inline QString name() const { return _name; }

        inline QString &name() { return _name; }

        inline bool saveFlag() const { return _save_flag; }

        inline bool &saveFlag() { return _save_flag; }

        QStringList str() const;
        QStringList strClean() const;
    };

private:
    QList<Cell> _cells;
    int _head;

public:
    explicit Softcan(Message *message = nullptr);

    friend QDataStream &operator>>(QDataStream &stream, Softcan &softcan);

    void toCurve(Curve &curve);

    bool load(QFile &file);

    inline bool load(QFile &&file) { return load(file); }

    inline bool load(const QString &file)
    {
        QFile f(file);
        return load(f);
    }

    inline bool load(QString &&file) { return load(file); }

    inline Cell &operator[](int index)
    {
        Q_ASSERT(index >= 0 && index < _cells.size());
        return _cells[index];
    }

    inline const Cell &operator[](int index) const
    {
        Q_ASSERT(index >= 0 && index < _cells.size());
        return _cells[index];
    }

    inline Cell &at(int index)
    {
        Q_ASSERT(index >= 0 && index < _cells.size());
        return _cells[index];
    }

    inline const Cell &at(int index) const
    {
        Q_ASSERT(index >= 0 && index < _cells.size());
        return _cells[index];
    }

    inline Cell &cell()
    {
        Q_ASSERT(_head >= 0);
        return _cells[_head];
    }

    inline const Cell &cell() const
    {
        Q_ASSERT(_head >= 0);
        return _cells[_head];
    }

    inline int size() const { return _cells.size(); }

    void add();

    QStringList tab() const;

    QStringList str() const;

#define SOFTCAN_MAGIC_LEN 22
    static bool checkMagic(const char *array);

private:

    inline void clear()
    {
        _cells.clear();
        _head = -1;
    }
};



QDataStream &operator>>(QDataStream &stream, Softcan &softcan);

QDataStream &operator>>(QDataStream &stream, Softcan::Cell &cell);


#endif //CORE_SOFTCAN_H
