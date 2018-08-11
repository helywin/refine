//
// Created by jiang.wenqiang on 2018/7/5.
//

#include "Sublime.h"
#include "Log.h"
#include "Transform.h"

Sublime::Sublime(Buffer &buffer, Kebab &kebab, Transform &transform) :
        buffer(buffer), kebab(kebab), transform(transform) {
    omp_init_lock(&lock);
}

Sublime::~Sublime() {
    omp_destroy_lock(&lock);
}

void Sublime::run() {
    qDebug("处理数据");
    kebab.add(transform.canToData(buffer));
    if (kebab.length() > KEBAB_CELL_LENGTH / 4) {
//    if (cells.length() > 100) {
        emit unload();
    }
}




