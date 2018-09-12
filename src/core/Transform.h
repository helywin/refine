//
// Created by jiang.wenqiang on 2018/9/11.
//

#ifndef REFINE_TRANSFORM_H
#define REFINE_TRANSFORM_H

#include <QtCore/QThread>
#include <QtCore/QFile>
#include <iostream>

class Buffer;
class Curve;

class Transform : public QThread
{
Q_OBJECT
private:
    Buffer *_buffer;
    Curve *_curve;
    QFile *_file_data;
    QFile *_file_frames;
    bool _save_frames;
public:
    Transform(Buffer *buffer, Curve *curve, QFile *file_data,
              QFile *file_frames = nullptr, bool save_frames = false);
};


#endif //REFINE_TRANSFORM_H
