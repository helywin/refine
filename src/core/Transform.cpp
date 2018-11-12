/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/15
 * @brief 报文转曲线
 * @details 报文根据曲线配置转换成曲线数据，同时补充丢失曲线数据
 ******************************************************************************/

#include "Transform.hpp"
#include "Curve.hpp"
#include "Tribe.hpp"

Transform::Transform() :
        _curve(nullptr),
        _buffer(nullptr),
        _tribe(nullptr),
        _file(),
        _status(Stop),
        _cmd(None) {}

void Transform::setParams(Curve *curve, Buffer *buffer, Tribe *tribe,
                          unsigned long msec)
{
    _curve = curve;
    _buffer = buffer;
    _tribe = tribe;
    _msec = msec;
}

void Transform::run()
{
    while (_cmd != CommandStop) {
        msleep(_msec);
        if (_cmd == CommandPause) {
            if (_status == Running) {
                _tribe->newSegment();
                _status = Pause;
            }
            continue;
        }
        if (_cmd == CommandResume &&
            _status == Pause) {
            _status = Running;
        }
//#define TEST_SEC    //测试时间消耗，发现主要开销来自线程创建和销毁
#ifdef TEST_SEC
        QTime t = QTime::currentTime();
#endif
        _tribe->setUnFilled();
        Buffer::Cell &buf = *_buffer->last();
        if (_buffer->isEmpty()) {
            continue;
        }
        //可以并行计算
//#pragma omp parallel for
//    for (int k = 0; k < _tribe->size(); ++k) {
//        Tribe::Cell &tr = (*_tribe)[k];
        for (Tribe::Cell &tr : *_tribe) {
            const Curve::Cell &cur = (*_curve)[tr.name()];
            for (unsigned int i = 0; (i < buf.dataSize()) && !tr.fill(); ++i) {
                if (cur.canId() == buf[i]->ID &&
                    (cur.zeroByte() == -1 ||
                     cur.zeroByte() == buf[i]->Data[0])) {
                    unsigned int high_byte = 0;
                    unsigned int low_byte = 0;
                    if (cur.highByte() != -1) {
                        high_byte = buf[i]->Data[cur.highByte()];
                        high_byte <<= 7 - cur.highByteRange()[1];
                        high_byte >>= 7 - cur.highByteRange()[1] +
                                      cur.highByteRange()[0];
                        high_byte <<= cur.lowByteRange()[1] -
                                      cur.lowByteRange()[0] + 1;
                    } else {
                        high_byte = 0;
                    }
                    low_byte = buf[i]->Data[cur.lowByte()];
                    low_byte <<= 7 - cur.lowByteRange()[1];
                    low_byte >>= 7 - cur.lowByteRange()[1] +
                                 cur.lowByteRange()[0];
                    unsigned int full = high_byte + low_byte;
                    float k;
                    float b;
                    k = (float) (cur.rangeOut()[1] -
                                 cur.rangeOut()[0]) /
                        (float) (cur.rangeIn()[1] - cur.rangeIn()[0]);
                    b = (float) cur.rangeOut()[0] -
                        k * cur.rangeIn()[0];
                    float result = full * k + b;
                    tr.push(Tribe::Data, result);
                }
            }
            if (!tr.fill()) {
                if (tr.empty()) {
//                    qDebug() << "Transform::run fake 0";
                    tr.push(Tribe::FakeByZero, 0);
                } else {
//                    qDebug() << tr.data().constLast();
                    tr.push(Tribe::FakeByPrevious, tr.data().constLast());
                }
            }
        }
#ifdef TEST_SEC
        qDebug() << t.msecsTo(QTime::currentTime());
#endif
        ++(*_tribe);
    }
}

void Transform::begin()
{
    _cmd = None;
    _tribe->reset();
    start(QThread::HighestPriority);
    _status = Running;
}

void Transform::stop(QFile *file)
{
    _cmd = CommandStop;
    while (isRunning()) {}
    if (file != nullptr) {
        _file.dumpCurveRecord(*file, *_tribe);
    }
    _status = Stop;
}

/*
#if 0
#ifdef way0
    _tribe->setUnFilled();
    Buffer::Cell &buf = *_buffer->last();
    if (_buffer->isEmpty()) {
        return;
    }
    for (int i = 0; i < buf.dataSize(); ++i) {
        QList<int> index_list;
        if (buf[i]->ID == 0x777) {
            index_list = _curve->findMap777(buf[i]->Data[0]);
        } else {
            index_list = _curve->findOtherMap(buf[i]->ID);
        }
        for (const auto index : index_list) {
            if ((*_tribe)[index].fill()) {
                continue;   //同一曲线多的数据丢弃
            }
            auto cur = (*_curve)[index];
            unsigned int full;
            unsigned int high_byte;
            unsigned int low_byte;
            if (cur.highByte() != -1) {
                high_byte = buf[i]->Data[cur.highByte()];
                high_byte <<= 7 - cur.highByteRange()[1];
                high_byte >>= 7 - cur.highByteRange()[1] +
                              cur.highByteRange()[0];
                high_byte <<= cur.lowByteRange()[1] -
                              cur.lowByteRange()[0] + 1;
            } else {
                high_byte = 0;
            }
            low_byte = buf[i]->Data[cur.lowByte()];
            low_byte <<= 7 - cur.lowByteRange()[1];
            low_byte >>= 7 - cur.lowByteRange()[1] +
                         cur.lowByteRange()[0];
            full = high_byte + low_byte;
            float k;
            float b;
            k = (float) (cur.rangeOut()[1] -
                         cur.rangeOut()[0]) /
                (float) (cur.rangeIn()[1] - cur.rangeIn()[0]);
            b = (float) cur.rangeOut()[0] -
                k * cur.rangeIn()[0];
            float result = full * k + b;
            (*_tribe)[cur.name()].push(Tribe::Data, result);
        }
    }


    //补充没有的报文
    for (auto &iter : *_tribe) {
        if (!iter.fill()) {
            if (iter.empty()) {
                iter.push(Tribe::FakeByZero, 0);
            } else {
                iter.push(Tribe::FakeByPrevious, iter.data().last());
            }
        }
    }
#else
#endif
*/

