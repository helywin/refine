//
// Created by jiang.wenqiang on 2018/9/11.
//

#ifndef REFINE_CSV_H
#define REFINE_CSV_H

#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QTextStream>
#include <QtCore/QTextCodec>

/*!
 * @brief csv文件读写的抽象
 */

class Csv
{
private:
    QFile *_file;
    QTextStream *_stream;
    QChar _separator;
public:
    explicit Csv(QFile *f = nullptr);
    ~Csv();
    void setSeparator(QChar sep);
    void setFile(QFile *f);
    bool startWrite(const char *codec, bool is_append = false);
    void writeLine(const QStringList &list) const;
    bool finishWrite();
    bool startRead(const char *codec);
    void readLine(QStringList &list) const;
    bool finishRead();

};


#endif //REFINE_CSV_H
