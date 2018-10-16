//
// Created by jiang.wenqiang on 2018/10/15.
//

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

    void beginTransform(Curve *curve, Buffer *buffer, Tribe *tribe);

    void finishTransform(QFile *file);

protected:
    void run() override;
};


#endif //REFINE_TRANSFORM_HPP
