//
// Created by jiang.wenqiang on 2018/11/1.
//

#ifndef REFINE_PACKER_HPP
#define REFINE_PACKER_HPP

#include <QtCore/QStringList>
#include <QtCore/QProcess>

#include "Initializer.hpp"

class Packer : public QObject
{
Q_OBJECT
public:
private:
    const char *PASSWORD = "by_Refine";
    QProcess _process;
    Initializer *_init;
public:
    Packer(Initializer *init);
    bool compress(const QStringList &file_list, const QString &dist_name);

    bool uncompress(const QString &file, const QString &dist_path);

    void testzip();

};


#endif //REFINE_PACKER_HPP
