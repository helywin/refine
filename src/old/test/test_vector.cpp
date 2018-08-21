//
// Created by jiang.wenqiang on 2018/7/11.
//

#include <QtCore/QVector>
#include <QtCore/QDebug>

int main() {
    double a[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    QVector<double> va{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    auto buff = new double[va.size()];
    if (!va.empty()) {
        memcpy(buff, &va[0], va.size() * sizeof(double));
    }
    for (int i = 0; i < 10; ++i) {
        //qDebug() << buff[i];
    }
    QVector<double> vb(10);
    memcpy(vb.data(), buff, vb.size() * sizeof(double));
    for (int i = 0; i < 10; ++i) {
        //qDebug() << vb[i];
    }
    FILE *plot_pip = popen("gnuplot", "w");
    if (plot_pip) {
        qDebug("打开成功");
    }
//    fprintf(plot_pip, "set terminal postscript eps\n");
//    fflush(plot_pip);
//    fprintf(plot_pip, "set output \"sin.esp\"\n");
//    fflush(plot_pip);
    fprintf(plot_pip, "plot sin(x)\n");
    fflush(plot_pip);
    system("pause");
    return 0;
}