//
// Created by jiang.wenqiang on 2018/9/19.
//

#ifndef REFINE_REVOLVE_HPP
#define REFINE_REVOLVE_HPP

#include <QtCore/QObject>
#include "Collect.hpp"
#include "Transform.hpp"
#include "Trigger.hpp"

class Revolve : public QObject
{
Q_OBJECT
private:
    Collect *_collect;
    Transform *_transform;
    Trigger *_trigger;

public:
    Revolve(Collect *collect, Transform *transform, Trigger *trigger);
    void reset();
    void run();

public slots:
    void collectFramesGot();
};


#endif //REFINE_REVOLVE_HPP
