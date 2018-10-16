//
// Created by jiang.wenqiang on 2018/10/15.
//

#ifndef REFINE_TRANSFORM_HPP
#define REFINE_TRANSFORM_HPP

#include <QtCore/QThread>

class Curve;
class Buffer;
class Tribe;

/*!
 * @brief 报文转换类
 */

class Transform : public QThread
{
Q_OBJECT
private:
    Curve *_curve;
    Buffer *_buffer;
    Tribe *_tribe;

public:
    Transform() = delete;
    Transform(Curve *curve, Buffer *buffer, Tribe *tribe);

    inline void setCurve(Curve *curve)
    {
        Q_ASSERT(curve != nullptr);
        _curve = curve;
    }

    inline void setBuffer(Buffer *buffer)
    {
        Q_ASSERT(buffer != nullptr);
        _buffer = buffer;
    }

    inline void setTribe(Tribe *tribe)
    {
        Q_ASSERT(tribe != nullptr);
        _tribe = tribe;
    }

protected:
    void run() override;
};


#endif //REFINE_TRANSFORM_HPP
