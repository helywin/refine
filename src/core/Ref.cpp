//
// Created by jiang.wenqiang on 2018/8/15.
//

#include "Ref.h"
#include "Version.h"


Ref::Ref() {
    _header.magic[0] = 89;
    _header.magic[1] = 'R';
    _header.magic[2] = 'E';
    _header.magic[3] = 'F';
    _header.magic[4] = Version::major;
    _header.magic[5] = Version::minor;
    _header.magic[6] = Version::micro;
    _header.magic[7] = 0;
}

