//
// Created by jiang.wenqiang on 2018/7/31.
//

#ifndef REFINE_VERSION_H
#define REFINE_VERSION_H

#include <QtCore/QString>

class Version {
private:
    enum Identifier {
        Alpha = 0x00,
        Beta = 0x01,
        Gamma = 0x02,
        Release = 0x03,
        Demo = 0x0F
    };

private:
    const static unsigned char _major = 0;
    const static unsigned char _micro = 0;
    const static unsigned char _minor = 1;
    const static unsigned char _build[4] = {18, 9, 11, 1};
    const static Identifier _identifier = Identifier::Demo;

public:
    Version();

    static const QString str() const;

    static unsigned char major() const;

    static unsigned char micro() const;

    static unsigned char minor() const;

    static const unsigned char *build() const;

    static unsigned char identifier() const;
};

#endif //REFINE_VERSION_H
