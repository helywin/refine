//
// Created by jiang.wenqiang on 2018/9/11.
//

#include <QtWidgets/QApplication>
#include "Version.h"

Version::Version()
{
    QApplication::setApplicationDisplayName("refine");
    QApplication::setOrganizationDomain("byd.com.cn");
    QApplication::setApplicationName("refine");
    QApplication::setOrganizationName("byd");
}

const QString Version::str() const
{
    QString s = QString("Version: %1.%2.%3.%4_%5%6%7_%8 ")
            .arg(_major)
            .arg(_micro)
            .arg(_minor)
            .arg(_build[0], 2, 10, QChar('0'))
            .arg(_build[1], 2, 10, QChar('0'))
            .arg(_build[2], 2, 10, QChar('0'))
            .arg(_build[3], 2, 10, QChar('0'));
    switch (_identifier) {
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

unsigned char Version::major() const
{
    return _major;
}

unsigned char Version::micro() const
{
    return _micro;
}

unsigned char Version::minor() const
{
    return _minor;
}

const unsigned char *Version::build() const
{
    return _build;
}

unsigned char Version::identifier() const
{
    return _identifier;
}
