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

char Version::major()
{
    return _major;
}

char Version::micro()
{
    return _micro;
}

char Version::minor()
{
    return _minor;
}

char Version::build()
{
    return _build;
}

char Version::identifier()
{
    return _identifier;
}

QDate Version::date()
{
    return QDate(_year + 2000, _month, _day);
}

char Version::year()
{
    return _year;
}

char Version::month()
{
    return _month;
}

char Version::day()
{
    return _day;
}
