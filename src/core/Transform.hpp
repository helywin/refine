//
// Created by jiang.wenqiang on 2018/9/11.
//

#ifndef REFINE_TRANSFORM_HPP
#define REFINE_TRANSFORM_HPP

#include <QtCore/QThread>
#include <QtCore/QFile>
#include <iostream>
#include "File.hpp"
#include "Tribe.hpp"

class Buffer;
class Curve;

class Transform : public QThread
{
Q_OBJECT
private:
    Buffer *_buffer;
    Curve *_curve;
    Tribe *_tribe;
    QFile *_file_frames;
    bool _frames_stored;
    File _file;

public:
    Transform() = delete;
    Transform(Buffer *buffer, Curve *curve, Tribe *tribe,
              QFile *file_frames = nullptr, bool save_frames = false);

    inline bool isFramesStored() { return _frames_stored; }

    inline void enableFramesStored(QFile *file)
    {
        _frames_stored = true;
        _file_frames = file;
    }

    inline void disableFramesStored() { _frames_stored = false; }

    inline bool initializeFramesStored()
    {
        return _file.dumpFrameRecordBegin(*_file_frames);
    }

    inline void finishFramesStored() {
        _file.dumpFrameRecordFinish(*_file_frames);
    }

    inline void reset() {}

protected:
    void run() override;
};


#endif //REFINE_TRANSFORM_HPP
