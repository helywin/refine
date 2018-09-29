//
// Created by jiang.wenqiang on 2018/9/19.
//

#ifndef REFINE_REVOLVE_HPP
#define REFINE_REVOLVE_HPP

#include <QtCore/QObject>
#include <QtCore/QThread>
#include "Collect.hpp"
#include "Transform.hpp"
#include "Trigger.hpp"

class Revolve : public QThread
{
Q_OBJECT
private:
    Collect *_collect;
    Transform *_transform;
    Trigger *_trigger;
    Tribe *_tribe;

public:
    Revolve(Collect *collect, Transform *transform, Trigger *trigger,
            Tribe *tribe);
    void reset();

protected:
    void run() override;

public slots:

    inline void startRevolve() { start(HighestPriority); }

    inline void pauseRevolve()
    {
        _collect->suspendCollection();
        while (_transform->isRunning()) {}
        _tribe->addGap();
    }

    inline void stopRevolve() { _collect->stopCollection(); }

    inline void resumeRevolve() { _collect->resumeCollection(); }

    void collectFramesGot();
};


#endif //REFINE_REVOLVE_HPP
