//
// Created by jiang.wenqiang on 2018/7/31.
//

#ifndef REFINE_VERSION_H
#define REFINE_VERSION_H

#include <QtCore/QString>
#include <QtCore/QDate>

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
    const static char _minor = 1;
    const static char _build = 1;
    const static char _year = 18;
    const static char _month = 9;
    const static char _day = 12;
    const static Identifier _identifier = Identifier::Demo;

public:
    Version() = default;

public:
    static QString str();
    static char major();
    static char micro();
    static char minor();
    static char build();
    static char year();
    static char month();
    static char day();
    static QDate date();
    static char identifier();
};

#endif //REFINE_VERSION_H
