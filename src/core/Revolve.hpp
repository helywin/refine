//
// Created by jiang.wenqiang on 2018/10/15.
//

#ifndef REFINE_REVOLVE_HPP
#define REFINE_REVOLVE_HPP

#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QFile>
#include "Can.hpp"
#include "Curve.hpp"
#include "File.hpp"
#include "Softcan.hpp"
#include "Transform.hpp"
#include "Tribe.hpp"
#include "Trigger.hpp"
#include "Buffer.hpp"
#include "Collect.hpp"
#include "Csv.hpp"
#include "Record.hpp"
#include "Despatch.hpp"
#include "Initializer.hpp"

/*!
 * @brief 底层调度类
 */

class Revolve : public QThread
{
Q_OBJECT
private:
    Can _can;
    Curve _curve;
    Buffer _buffer;
    Collect _collect;
    Tribe _tribe;
    Transform _transform;
    Record _record;
    Revolve _revolve;
    Softcan _softcan;
    QTimer _timer;
    QFile *_store_frames;
    QFile *_collect_frames;
    QFile *_store_curves;
    int _msec;
    bool _is_transform;
    bool _is_record;

public:
    Revolve();




public slots:
    void marvel(bool is_transform = true, bool is_record = true);

private slots:
    void tictoc();

    void CollectError(int code);

    void TransformError(int code);

    void RecordError(int code);

};


#endif //REFINE_REVOLVE_HPP
