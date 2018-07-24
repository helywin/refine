//
// Created by jiang.wenqiang on 2018/7/17.
//

#ifndef CORE_INTERFACE_H
#define CORE_INTERFACE_H

#include "Rate.h"

class Interface {
public:
    class PdfInterface {
    public:
        typedef void *(PdfHandler)(Rate &rate);
    };
};


#endif //CORE_INTERFACE_H
