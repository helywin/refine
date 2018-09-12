//
// Created by jiang.wenqiang on 2018/9/11.
//

#include "Version.h"


QString Version::str()
{
    QString s = QString("Version: %1.%2.%3.%4_%5%6%7_%8 ")
            .arg(_major)
            .arg(_micro)
            .arg(_minor);
    switch (_identifier) {
        case Identifier::None:
            break;
        case Identifier::Demo:
            s += QString("(Demo)");
            break;
        case Identifier::Alpha:
            s += QString("(Alpha)");
            break;
        case Identifier::Beta:
            s += QString("(Beta)");
            break;
        case Identifier::Gamma:
            s += QString("(Gamma)");
            break;
        case Identifier::Release:
            s += QString("(Release)");
            break;
    }
    return qMove(s);
}

unsigned char Version::major()
{
    return _major;
}

unsigned char Version::micro()
{
    return _micro;
}

unsigned char Version::minor()
{
    return _minor;
}

unsigned char Version::build()
{
    return _build;
}

unsigned char Version::identifier()
{
    return _identifier;
}

QDate Version::date()
{
    return QDate(_year + 2000, _month, _day);
}

unsigned char Version::year()
{
    return _year;
}

unsigned char Version::month()
{
    return _month;
}

unsigned char Version::day()
{
    return _day;
}
