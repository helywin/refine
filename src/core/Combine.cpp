//
// Created by jiang.wenqiang on 2018/11/28.
//

#include "Combine.hpp"

Combine::Combine() :
        _len(0) {}

void Combine::genFromTribe(const Tribe &tribe)
{
    reset(tribe.size());
    for (int i = 0; i < tribe.size(); ++i) {
        for (int j = 0; j < tribe.len(); ++j) {
            auto y = (float)
                    ((tribe[i].data()[j] - tribe.style(i).rangeOut()[0]) * (Y_POINTS) /
                     (tribe.style(i).rangeOut()[1] - tribe.style(i).rangeOut()[0]) + 0);
            append(i, (j - 1) * tribe.msec() / 1000.0f, y);
        }
    }
    if (_cells.isEmpty()) {
        _len = 0;
        return;
    }
    _len = _cells[0].data().size();
    for (const auto &cell : _cells) {
        if (_len > cell.data().size()) {
            _len = cell.data().size();
        }
    }
}