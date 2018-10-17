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
    File _file;

public:
    Transform();

    void setParams(Curve *curve, Buffer *buffer, Tribe *tribe);

    inline void begin(Curve *curve, Buffer *buffer, Tribe *tribe)
    {
        setParams(curve, buffer, tribe);
        begin();
    }

    void begin() {}

    void finish(QFile *file = nullptr);

protected:
    void run() override;
};


#endif //REFINE_TRANSFORM_HPP
