/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/15
 * @brief 报文转曲线
 * @details 报文根据曲线配置转换成曲线数据，同时补充丢失曲线数据
 ******************************************************************************/

#ifndef REFINE_TRANSFORM_HPP
#define REFINE_TRANSFORM_HPP

#include <QtCore/QDebug>
#include <QtCore/QThread>
#include <QtCore/QFile>
#include "File.hpp"
#include "RecvBuffer.hpp"
#include "Message.hpp"
#include "Global.hpp"

class Curve;
class RecvBuffer;
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
    Curve *_curve = nullptr;
    RecvBuffer *_buffer = nullptr;
    Tribe *_tribe = nullptr;
    File _file;
    unsigned long _msec = 10;
    Re::RunningStatus _status = Re::Stop;
    Re::RunningCommand _cmd = Re::NoCommand;
    QByteArray _bytes;
    bool _transform_curve = false;
    QMap<unsigned int, bool> _recv_id = {{0x613, true},
                                         {0x335, true}};

public:
    explicit Transform(Message *message = nullptr);

    void setParams(Curve *curve, RecvBuffer *buffer, Tribe *tribe,
                   unsigned long msec = 10);

    void begin();

    void stop(const QString &file = nullptr);

    inline void pause()
    { _cmd = Re::CommandPause; }

    inline void resume()
    { _cmd = Re::CommandResume; }

    inline int status() const
    { return _status; }

    inline void setTransformCurve(bool transform_curve)
    { _transform_curve = transform_curve; }

public slots:

    inline void setRecvId(const QMap<unsigned int, bool> &ids)
    { _recv_id = ids; }

protected:
    void run() override;

signals:
    void getTransformedCanMessage(const QByteArray &message, unsigned int id);
    void getNewRecvId(unsigned int id);
};


#endif //REFINE_TRANSFORM_HPP
