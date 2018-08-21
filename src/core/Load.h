//
// Created by jiang.wenqiang on 2018/8/20.
//

#ifndef REFINE_LOAD_H
#define REFINE_LOAD_H

#include <QtCore/QFile>
#include <QtCore/QThread>
#include "Tribe.h"
#include "Csv.h"
#include "Rf.h"


class Load : public QThread{
Q_OBJECT
public:
    enum FileType {
        Csv,
        Rf,
    };
private:
    Tribe *_tribe;
    QFile *_file;
    FileType _file_type;
    ::Csv *_csv;
    ::Rf *_rf;

public:
    Load();

    void setTribe(Tribe *tribe);

    void setFile(QFile *f, FileType type);

protected:
    void run() override;
};


#endif //REFINE_LOAD_H
