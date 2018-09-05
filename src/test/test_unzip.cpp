//
// Created by jiang.wenqiang on 2018/9/5.
//

#include <QtCore/QDebug>
#include <QtWidgets/QApplication>
#include <zip.h>
#include <zlib.h>

int main(int argc, char **argv) {
    zip_stat_t a;
    zip *z;
    zip_error error = {};
    int e = 0;
    z = zip_open("D:/jiang.wenqiang/Desktop/unc.zip", ZIP_CREATE, &e);
    qDebug() << ZIP_ER_INCONS << ":" << e;
    zip_source *zs;
    zs = zip_source_file_create("D:/jiang.wenqiang/Desktop/unc.txt", 0, 0, &error);
    zip_add(z, "12", zs);
    zip_close(z);
//    int file_count = zip_get_num_files(z);
//    qDebug() << file_count;
//    zip_flags_t flag = 0;
//    zip_stat_init(&a);
//    zip_stat_index(z, 0, 0, &a);
//    qDebug() << error;
//    qDebug() << a.name;
    return 0;
}