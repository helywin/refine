/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/11
 * @brief
 * @details
 ******************************************************************************/

#ifndef REFINE_DESPATCH_HPP
#define REFINE_DESPATCH_HPP

#include <QtCore/QObject>


class Collect;
class Transform;
class Trigger;

/*!
 * @brief 判断工况是否触发的线程
 */

class Despatch : public  QObject
{
Q_OBJECT
private:
    Collect *_collect;
    Transform *_transform;
    Trigger *_trigger;
};


#endif //REFINE_DESPATCH_HPP
