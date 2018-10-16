//
// Created by jiang.wenqiang on 2018/7/31.
//

#ifndef REFINE_VERSION_HPP
#define REFINE_VERSION_HPP

#include <QtCore/QString>
#include <QtCore/QDate>

/*!
 * @brief 记录软件详细信息的类
 * 后续加上其他版本信息，比如特性
 */

class Version
{
public:
    enum Identifier
    {
        None = 0x00,
        Demo = 0x01,
        Alpha = 0x02,
        Beta = 0x04,
        Gamma = 0x08,
        Release = 0x10
    };

private:
    const static char _major = 0;
    const static char _micro = 0;
    const static char _minor = 3;
    const static char _build = 1;
    const static char _year = 18;
    const static char _month = 10;
    const static char _day = 11;
    const static Identifier _identifier = Identifier::Demo;

public:
    static QString str();
    inline static char major() { return _major; }
    inline static char micro() { return _micro; }
    inline static char minor() { return _minor; }
    inline static char build() { return _build; }
    inline static char year() { return _year; }
    inline static char month() { return _month; }
    inline static char day() { return _day; }
    inline static QDate date() { return QDate(_year + 2000, _month, _day); }
    inline static char identifier() { return _identifier; }
};

#endif //REFINE_VERSION_HPP
