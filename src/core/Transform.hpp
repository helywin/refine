/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/15
 * @brief 报文转曲线
 * @details 报文根据曲线配置转换成曲线数据，同时补充丢失曲线数据
 ******************************************************************************/

#ifndef REFINE_TRANSFORM_HPP
#define REFINE_TRANSFORM_HPP

#include <QtCore/QThread>
#include <QtCore/QFile>
#include "File.hpp"
#include "Buffer.hpp"

class Curve;
class Buffer;
class Tribe;

/*!
 * @brief 报文转换类
 */

class Transform : public QThread
{
Q_OBJECT
public:

    enum Status
    {
        Stop,
        Running,
        Pause
    };

    enum Command
    {
        None,
        CommandStop,
        CommandResume,
        CommandPause
    };

private:
    Curve *_curve;
    Buffer *_buffer;
    Tribe *_tribe;
    File _file;
    unsigned long _msec;
    Status _status;
    Command _cmd;

public:
    Transform();

    void setParams(Curve *curve, Buffer *buffer, Tribe *tribe,
                   unsigned long msec = 10);

    void begin();

    void stop(QFile *file = nullptr);

    inline void pause() { _cmd = CommandPause; }

    inline void resume() { _cmd = CommandResume; }

    inline int status() const { return _status; }

protected:
    void run() override;
};


#endif //REFINE_TRANSFORM_HPP
