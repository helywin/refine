//
// Created by jiang.wenqiang on 2018/9/11.
//

#ifndef REFINE_COLLECT_H
#define REFINE_COLLECT_H

#include <QtCore/QThread>
#include <QtCore/QFile>
#include "File.h"

class Can;
class Buffer;

class Collect : public QThread
{
Q_OBJECT
public:
    enum CollectManner
    {
        CollectFromCanWithDelay = 0x00,
        CollectFromFileWithDelay = 0x01,
        CollectFromFileWithoutDelay = 0x02
    };

private:
    Can *_can;
    Buffer *_buffer;
    CollectManner _manner;
    int _delay;
    File *_file;

public:
    Collect() = delete;
    Collect(Can *can, Buffer *buffer);
    void setMode(CollectManner manner, int delay = 10, File *file = nullptr);
    void setDelay(int delay);
    void setFile(File *file);

protected:
    void run() override;
};


#endif //REFINE_COLLECT_H
