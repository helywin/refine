//
// Created by jiang.wenqiang on 2018/7/31.
//

#ifndef REFINE_VERSION_H
#define REFINE_VERSION_H

#include <QtCore/QString>

class Version {
private:
    enum class Id {
        Demo = -1,
        Alpha = 0,
        Beta = 1,
        Gamma = 2,
        Release = 3,
    };

private:
    const static int major = 0;
    const static int micro = 0;
    const static int minor = 0;
    const static Id identifier = Id::Demo;

public:
    static const QString str() {
        QString s = QString("Version: %1.%2.%3").arg(major).arg(micro).arg(minor);
        switch (identifier) {
            case Id::Demo:s += QString("(Demo)");break;
            case Id::Alpha:s += QString("(Alpha)");break;
            case Id::Beta:s += QString("(Beta)");break;
            case Id::Gamma:s += QString("(Gamma)");break;
            case Id::Release:s += QString("(Release)");break;
        }
        return s;
    }
};

#endif //REFINE_VERSION_H
