//
// Created by jiang.wenqiang on 2018/7/5.
//

#include "Sublime.h"
#include "Log.h"

Sublime::Sublime(CurveConfig &cfg, CanBuffer &buffer, Kebab &kebab) :
        cfg(cfg), buffer(buffer), kebab(kebab) {
    omp_init_lock(&lock);
}

Sublime::~Sublime() {
    omp_destroy_lock(&lock);
}

void Sublime::run() {
    qDebug("处理数据");
    unsigned int use = 0;
    unsigned int total = 0;
    while (auto p = buffer.out()) {
        total += p->l;
#pragma omp parallel for
        for (int i = 0; i < p->l; ++i) {
            unsigned short index = 0;
            double result;
            if (cfg.transform(p->buffer->ID,
                              p->buffer->Data,
                              result, index)) {
                int index_u = index - 1;
                Q_ASSERT(index_u >= 0);
                kebab.add((unsigned short)index_u, result);
                omp_set_lock(&lock);
//                qDebug() << "曲线" << index << "添加" << result;
                use += 1;
//                qDebug() << use;
                omp_unset_lock(&lock);
            }
        }
    }
    emit result(use, total);
    if (kebab.length() > KEBAB_CELL_LENGTH / 4) {
//    if (kebab.length() > 100) {
        emit unload();
    }
}




