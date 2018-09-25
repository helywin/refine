//
// Created by jiang.wenqiang on 2018/9/11.
//

#include "Tribe.hpp"

void Tribe::trim()
{
    int size = -1;
    for (const auto &iter : _cells) {
        if (size == -1) {
            size = iter.size();
            continue;
        }
        if (size > iter.size()) {
            size = iter.size();
        }
    }
    for (auto &iter : _cells) {
        iter.resize(size);
    }
}
