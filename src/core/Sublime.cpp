//
// Created by jiang.wenqiang on 2018/7/5.
//

#include "Sublime.h"
#include "Log.h"

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
        emit unload();
    }
}




