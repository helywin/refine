//
// Created by jiang.wenqiang on 2018/7/11.
//

#include <cstdio>
#include <cmath>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_fft_halfcomplex.h>
#include <gsl/gsl_fft_complex.h>

int main() {
    int i = 101;
    const size_t n = 101;
    double data[n];
    gsl_fft_real_wavetable *real;
    gsl_fft_halfcomplex_wavetable *hc;
    gsl_fft_real_workspace *work;
    //! \brief 原始方波
    for (i = 0; i < n; i++) {
        data[i] = 0.0;
    }
    for (i = n / 3; i < 2 * n / 3; i++) {
        data[i] = 1.0;
    }
    for (i = 0; i < n; i++) {
        printf("%e\n", data[i]);
    }
    printf("\n");
    work = gsl_fft_real_workspace_alloc(n);
    real = gsl_fft_real_wavetable_alloc(n);
    gsl_fft_real_transform(data, 1, n, real, work);
    gsl_fft_real_wavetable_free(real);

    printf("===================\n");
    for (i = 0; i < n; i++) {
        printf("%e\n", data[i]);
    }
    //! \brief 测试得到频谱的范围为0~Fs/2,单边频谱,计算点数是注意
    printf("===================\n");
    //! \brief 滤波器窗
    for (i = 11; i < n; i++) {
        data[i] = 0;
    }
    hc = gsl_fft_halfcomplex_wavetable_alloc(n);
    gsl_fft_halfcomplex_inverse(data, 1, n, hc, work);
    gsl_fft_halfcomplex_wavetable_free(hc);
    for (i = 0; i < n; i++) {
        printf(/*"%d:*/ "%e\n", /*i,*/ data[i]);
    }
    gsl_fft_real_workspace_free(work);
    return 0;
}
