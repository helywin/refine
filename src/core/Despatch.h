//
// Created by jiang.wenqiang on 2018/9/11.
//

#ifndef REFINE_DESPATCH_H
#define REFINE_DESPATCH_H

#include <QtCore/QObject>


class Collect;
class Transform;
class Trigger;

class Despatch : public  QObject
{
Q_OBJECT
private:
    Collect *_collect;
    Transform *_transform;
    Trigger *_trigger;
};


#endif //REFINE_DESPATCH_H
