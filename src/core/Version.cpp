//
// Created by jiang.wenqiang on 2018/9/11.
//

#include "Version.hpp"


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
