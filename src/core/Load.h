//
// Created by jiang.wenqiang on 2018/8/20.
//

#ifndef REFINE_LOAD_H
#define REFINE_LOAD_H

#include <QtCore/QFile>
#include <QtCore/QThread>
#include "Tribe.h"
#include "Csv.h"
#include "Ref.h"


class Load : public QThread{
Q_OBJECT
public:
    enum class FileType {
        Csv,
        Ref,
    };
private:
    Tribe *_tribe;
    QFile *_file;
    FileType _file_type;
    Csv *_csv;
    Ref *_ref;

public:
    Load();

    void setTribe(Tribe *tribe);

    void setFile(QFile *f, FileType type);

protected:
    void run() override;
};


#endif //REFINE_LOAD_H
