//
// Created by jiang.wenqiang on 2018/7/10.
//

#ifndef CORE_REFINE_H
#define CORE_REFINE_H

#include "Tribe.h"

class Range {
private:
    unsigned int begin_index;
    unsigned int length;
public:
    Range() = delete;

    Range(const unsigned int begin_index, const unsigned int length) :
            begin_index(begin_index), length(length) {};

    inline unsigned int begin() { return begin_index; }

    //end仅仅用来循环使用,循环中i < x.end(),不用加等号
    inline unsigned int end() { return begin_index + length; }

    inline unsigned int len() { return length; }
};

class Refine {
public:
    static void smoothing(const TribeCell &in, TribeCell &out,
                          unsigned int point);

    static void derivation(const TribeCell &in, TribeCell &out);

    static void filter(const TribeCell &in, TribeCell &out,
                       double s_freq, double l_freq,
                       unsigned int h_freq, bool band_pass);

    static double average(const TribeCell &in, Range &range);

    static double rms(const TribeCell &in, Range &range);

    static double vdv(const TribeCell &in, Range &range);

private:
    static inline unsigned int freqToIndex(double s_freq, double freq,
                                           unsigned int point);
};


#endif //CORE_REFINE_H
