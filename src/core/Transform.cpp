//
// Created by jiang.wenqiang on 2018/7/30.
//

#include "Transform.h"
#include "ControlCan.h"

Transform::Transform(Curve &config) : config(config) {
}

Kebab::Group &&Transform::canToData(Buffer &buffer) const {
    Kebab::Group group;
    while (Buffer::Cell *cell = buffer.out()) {
        for (int j = 0; j < cell->length(); ++j) {
            unsigned short index = 0;
            double result;
            for (const auto &iter : config._config) {
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
                    double k;
                    double b;
                    k = (double) (iter.range_out[1] - iter.range_out[0]) /
                        (double) (iter.range_in[1] - iter.range_in[0]);
                    b = (double) iter.range_out[0] - k * iter.range_in[0];
                    result = (double) full * k + b;
                    index = iter.index;
                    group[index] = result;
                }
            }

        }
    }
    return qMove(group);
}

Buffer::Cell &&Transform::dataToCan(Tribe &tribe) const {
    Buffer::Cell cell;


}
