/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/15
 * @brief 底层调度
 * @details 底层功能模块全部由Revolve实现调度，GUI不参与任何变量初始化和传递
 ******************************************************************************/

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
#include "Transmit.hpp"

/*!
 * @brief 底层调度类
 * 外部GUI只传文件名进来
 */

class Revolve : public QThread
{
Q_OBJECT
private:
    Can _can;
    Curve _curve;
    Buffer _buffer;
    Collect _collect;
    Transmit _transmit;
    Tribe _tribe;
    Transform _transform;
    Record _record;
    Softcan _softcan;
    QTimer _timer;
    QFile *_store_frames;
    QFile *_collect_frames;     //从GUI读取的量
    QFile *_store_curves;
    int _msec;
    bool _is_transform;
    bool _is_record;

public:
    Revolve();

    inline void setCollectManner(Collect::Manner manner,
                                 const QString &collect_frames_name)
    {

    }

public slots:
    void marvel(int msec = 10, bool is_transform = true,
                bool is_record = true);

    void stop();

private slots:
    void tictoc();

    void CollectError(int code);

    void TransformError(int code);

    void RecordError(int code);

};


#endif //REFINE_REVOLVE_HPP
