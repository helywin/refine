//
// Created by jiang.wenqiang on 2018/7/10.
//

#include "Refine.h"
#include "Log.h"
#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_fft_halfcomplex.h>

void Refine::smoothing(const TribeCell &in, TribeCell &out,
                       unsigned int point) {
    const unsigned int half = point / 2;
    double sum = 0;
    point += !(point % 2);
    Q_ASSERT(in.size() > point);
    if (out.size() != in.size()) {
        out.resize(in.size());
    }
    for (int i = 0; i < in.size(); ++i) {
        if (i < half || i > in.size() - half - 1) {
            out[i] = in[i];
        } else if (i == half) {
            for (int j = 0; j < point; ++ j) {
                sum += in[i];
            }
            sum /= point;
            out[i] = sum;
        } else {
            sum = sum + (in[i + half] - in[i - half - 1]) / point;
            out[i] = sum;
        }
    }
}

void Refine::derivation(const TribeCell &in, TribeCell &out) {
    Q_ASSERT(in.size() > 1);
    if (out.size() != in.size()) {
        out.resize(in.size());
    }
    out[0] = in[0];
    for (int i = 1; i < in.size(); ++i) {
        out[i] = in[i] - in[i - 1];
    }
}

void Refine::filter(const TribeCell &in, TribeCell &out,
                    double s_freq, double l_freq,
                    unsigned int h_freq, bool band_pass) {
    const auto size = (size_t)in.size();
    unsigned int index_low = freq_to_index(s_freq, l_freq, size);
    unsigned int index_high = freq_to_index(s_freq, h_freq, size);
    if (out.size() != in.size()) {
        out.resize(size);
    }
    //拷贝开始
    out = in;
    double *data = out.data();
    //拷贝完毕
    gsl_fft_real_wavetable *real;
    gsl_fft_halfcomplex_wavetable *hc;
    gsl_fft_real_workspace *work;
    //开始alloc;
    work = gsl_fft_real_workspace_alloc(size);
    real = gsl_fft_real_wavetable_alloc(size);
    hc = gsl_fft_halfcomplex_wavetable_alloc(size);
    //开始频域变换
    gsl_fft_real_transform(data, 1, size, real, work);
    gsl_fft_real_wavetable_free(real);
    //完成频域变换
    //开始设置滤波窗
    for (int i = 0; i < size; ++i) {
        if ((!band_pass && i < index_high && i > index_low) ||  //带阻
            (band_pass && (i > index_high || i < index_low))) { //带通
            out[i] = 0;
        }
    }
    //开始时域变换
    gsl_fft_halfcomplex_inverse(data, 1, size, hc, work);
    gsl_fft_halfcomplex_wavetable_free(hc);
    gsl_fft_real_workspace_free(work);
    //完成时域变换
}

unsigned int Refine::freq_to_index(double s_freq, double freq,
                                     unsigned int point) {
    auto result = (unsigned int)(freq/s_freq*point*2);
    return result > point? point : result;
}

double Refine::average(const TribeCell &in, Range &range) {
    double sum = 0;
    for (int i = range.begin(); i < range.end(); ++i) {
        sum += in[i];
    }
    return sum/range.len();
}

double Refine::rms(const TribeCell &in, Range &range) {
    double sum = 0;
    for (int i = range.begin(); i < range.end(); ++i) {
        sum += in[i] * in[i];
    }
    sum /= range.len();
    return sqrt(sum);
}

double Refine::vdv(const TribeCell &in, Range &range) {
    double sum = 0;
    double times = 0;
    for (int i = range.begin(); i < range.end(); ++i) {
        times =  in[i] * in[i];
        times *= times;
        sum += times;
    }
    return sqrt(sqrt(sum));
}