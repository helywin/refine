//
// Created by jiang.wenqiang on 2018/8/8.
//

#ifndef REFINE_DUMP_H
#define REFINE_DUMP_H

#include <QtCore/QThread>
#include <QtCore/QFile>
#include "Csv.h"
#include "Ref.h"

class Kebab;

class Dump : public QThread {
Q_OBJECT
public:
    enum FileType {
        Csv,
        Ref,
    };
private:
    Kebab *_kebab;
    QFile *_file;
    FileType _file_type;
    ::Csv *_csv;
    ::Ref *_ref;

public:
    Dump();

    void setKebab(Kebab *kebab);

    void setFile(QFile *f, FileType type);

protected:
    void run() override;
};


#endif //REFINE_DUMP_H
