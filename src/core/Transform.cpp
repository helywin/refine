//
// Created by jiang.wenqiang on 2018/7/30.
//

#include "Transform.h"
#include "ControlCan.h"

Transform::Transform(CurveConfig &config) : config(config) {
}

Kebab::Group &&Transform::canToData(CanBuffer &buffer) const {
    Kebab::Group group;
    while (CanBuffer::Cell *cell = buffer.out()) {
        for (int j = 0; j < cell->length(); ++j) {
            unsigned short index = 0;
            float result;
            for (const auto &iter : config.config) {
                bool flag = iter.can_id == cell[j].buffer()->ID &&
                            (!iter.zero_flag ||
                             cell[j].buffer()->Data[0] == iter.zero_byte);
                if (flag) {
                    unsigned int full;
                    unsigned int high_byte;
                    unsigned int low_byte;
                    if (iter.high_flag) {
                        high_byte = cell[j].buffer()->Data[iter.high_byte];
                        high_byte <<= 7 - iter.high_range[1];
                        high_byte >>= 7 - iter.high_range[1] + iter.high_range[0];
                        high_byte <<= iter.low_range[1] - iter.low_range[0] + 1;
                    } else {
                        high_byte = 0;
                    }
                    low_byte = cell[j].buffer()->Data[iter.low_byte];
//            qDebug() << "low byte " << low_byte;
                    low_byte <<= 7 - iter.low_range[1];
                    low_byte >>= 7 - iter.low_range[1] + iter.low_range[0];
                    full = high_byte + low_byte;
//            qDebug() << full;
                    float k;
                    float b;
                    k = (float) (iter.range_out[1] - iter.range_out[0]) /
                        (float) (iter.range_in[1] - iter.range_in[0]);
                    b = (float) iter.range_out[0] - k * iter.range_in[0];
                    result = (float) full * k + b;
                    index = iter.index;
                    group[index] = result;
                }
            }

        }
    }
    return qMove(group);
}

CanBuffer::Cell &&Transform::dataToCan(Tribe &tribe) const {
    CanBuffer::Cell cell;


}
