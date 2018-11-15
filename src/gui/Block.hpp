//
// Created by jiang.wenqiang on 2018/11/15.
//

#ifndef REFINE_BLOCK_HPP
#define REFINE_BLOCK_HPP

#include <QtCore/QThread>
#include "InfiniteProgress.hpp"

class Block : public QThread
{
private:
    InfiniteProgress _progress;
    QStringList _messages;
    int _index;
public:
    int waitForFinish(const QString &msg);
    void unblock(int index);
};


#endif //REFINE_BLOCK_HPP
