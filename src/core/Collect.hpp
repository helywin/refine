/*******************************************************************************
 * @file Revolve.cpp
 * @author jiang.wenqiang
 * @date 2018/10/15
 * @brief 底层调度
 * @details 底层功能模块全部由Revolve实现调度，GUI不参与任何变量初始化和传递
 ******************************************************************************/

#ifndef REFINE_COLLECT_HPP
#define REFINE_COLLECT_HPP

#include <QtCore/QThread>
#include <QtCore/QFile>
#include "Buffer.hpp"
#include "Can.hpp"
#include "File.hpp"

class Collect : public QThread
{
Q_OBJECT
public:
    enum Manner
    {
        FromCan,
        FromFile
    };

    enum Error
    {
        BufferFull,
        ConnectionLost,
        CanFailed,
        FileError
    };
private:
    Can *_can;
    Buffer *_buffer;
    Manner _manner;
    QFile *_frame_file;
    File _file;

public:
    Collect();

    void setParams(Can *can, Buffer *buffer,
                   Manner manner, QFile *frame_file = nullptr);

    bool begin();

    inline bool begin(Can *can, Buffer *buffer,
                      Manner manner, QFile *frame_file = nullptr)
    {
        setParams(can, buffer, manner, frame_file);
        return begin();
    }

    inline void finish() { _file.loadFrameRecordFinish(*_frame_file); }

    inline Manner manner() const { return _manner; }

protected:
    void run() override;

signals:
    void error(int error);

    void fileEnd();
};


#endif //REFINE_COLLECT_HPP
