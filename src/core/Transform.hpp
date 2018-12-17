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
#include "Message.hpp"
#include "Global.hpp"

class Curve;
class Buffer;
class Tribe;
class Combine;

/*!
 * @brief 报文转换类
 */

class Transform : public QThread, public Message
{
Q_OBJECT
public:

private:
    Curve *_curve;
    Buffer *_buffer;
    Tribe *_tribe;
    File _file;
    unsigned long _msec;
    Re::RunningStatus _status;
    Re::RunningCommand _cmd;

public:
    explicit Transform(Message *message = nullptr);

    void setParams(Curve *curve, Buffer *buffer, Tribe *tribe,
                   unsigned long msec = 10);

    void begin();

    void stop(const QString &file = nullptr);

    inline void pause() { _cmd = Re::CommandPause; }

    inline void resume() { _cmd = Re::CommandResume; }

    inline int status() const { return _status; }

protected:
    void run() override;

signals:
    void resetHScroll(int size, bool reset);
};


#endif //REFINE_TRANSFORM_HPP
