//
// Created by jiang.wenqiang on 2018/7/5.
//

#include "Sublime.h"
#include "Log.h"
#include "Transform.h"

<<<<<<< HEAD
Sublime::Sublime(CurveConfig &cfg, CanBuffer &buffer, Kebab &raft) :
        cfg(cfg), buffer(buffer), raft(raft) {}

void Sublime::run() {
    qDebug("处理数据");
    unsigned int use = 0;
    unsigned int total = 0;
    while (auto p = buffer.out()) {
        for (int i = 0; i < p->l; ++i) {
            unsigned short index = 0;
            double result;
            if (cfg.transform(p->buffer->ID,
                              p->buffer->Data,
                              result, index)) {
                raft.add(index, result);
                qDebug() << "曲线" << index << "添加" << result;
                use += 1;
            }
        }
    }
    emit result(use, total);
    if (raft.length() > RAFT_CELL_LENGTH/2) {
//    if (raft.length() > 100) {
=======
Sublime::Sublime(CanBuffer &buffer, Kebab &kebab, Transform &transform) :
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
>>>>>>> 568e96c... 准备重新开发结构
        emit unload();
    }
}




