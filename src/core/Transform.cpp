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
#include "Sketch.hpp"

Transform::Transform(Message *message) :
        Message(message),
        _curve(nullptr),
        _buffer(nullptr),
        _tribe(nullptr),
        _file(),
        _msec(10),
        _status(Re::Stop),
        _cmd(Re::NoCommand),
        _transform_curve(false)
{}

void Transform::setParams(Curve *curve, RecvBuffer *buffer, Tribe *tribe,
                          unsigned long msec)
{
    _curve = curve;
    _buffer = buffer;
    _tribe = tribe;
    _msec = msec;
    _tribe->setMsec((int) msec);
}

void Transform::run()
{
    while (_cmd != Re::CommandStop) {
        msleep(_msec);
        if (_cmd == Re::CommandPause) {
            if (_status == Re::Running) {
                _tribe->newSegment();
                _status = Re::Pause;
            }
            continue;
        }
        if (_cmd == Re::CommandResume
            && _status == Re::Pause) {
            _status = Re::Running;
        }
//#define TEST_SEC    //测试时间消耗，发现主要开销来自线程创建和销毁
#ifdef TEST_SEC
        QTime t = QTime::currentTime();
#endif
        _tribe->setUnFilled();
        RecvBuffer::Cell &buf = *_buffer->last();
        if (_buffer->isEmpty()) {
            continue;
        }

        //增加通信功能
        for (auto i = 0u; i < buf.dataSize(); ++i) {
            if (buf[i]->ID == 0x613) {
                QByteArray bytes((char *) buf[i]->Data, buf[i]->DataLen);
                getTransformedCanMessage(QString::fromLocal8Bit(bytes));
                bytes = bytes.replace("\r\n", "\n");
//                auto list = bytes.split('\n');
//                Q_ASSERT(!list.isEmpty());
//                if (list.size() == 1) {
//                    _bytes.append(bytes);
//                } else if (list.size() > 1) {
//                    for (int j = 0; j < list.size() - 1; ++j) {
//                        _bytes.append(list[j]);
//                        _bytes.clear();
//                    }
//                    _bytes = list.last();
//                }
            }
        }

        if (_status == Re::Pause || !_transform_curve) {
            continue;
        }

        //可以并行计算
//#pragma omp parallel for
//        for (int i = 0; i < _tribe->size(); ++i) {
//            Tribe::Cell &tr = (*_tribe)[i];
        for (auto &tr : *_tribe) {
            const Curve::Cell &cur = (*_curve)[tr.name()];
            for (unsigned int j = 0; (j < buf.dataSize()) && !tr.fill(); ++j) {
                if (static_cast<unsigned int>(cur.canId()) == buf[j]->ID &&
                    (cur.zeroByte() == -1 ||
                     cur.zeroByte() == buf[j]->Data[0])) {
#if 0
                    int value = 0;
                if (cur.highByte() != -1) {
                    unsigned char h = buf[i]->Data[cur.highByte()];
                    unsigned char l = buf[i]->Data[cur.lowByte()];
                    auto high_pos0 = (unsigned char) cur.highByteRange()[0];
                    auto f = (unsigned char) cur.highByteRange()[1];
                    auto s = (unsigned char) cur.lowByteRange()[0];
                    auto t = (unsigned char) cur.lowByteRange()[1];
                    value = ((((unsigned char) (h << (7 - f))) >> (7 - f)) << (t - s + 1)) +
                            (((unsigned char) (l << (7 - t))) >> (7 - t + s));
                    if (cur.rangeIn()[0] < 0) {
                        value = (signed short)(value);
                    }
                } else {
                    unsigned char l = buf[i]->Data[cur.lowByte()];
                    auto s = (unsigned char) cur.lowByteRange()[0];
                    auto t = (unsigned char) cur.lowByteRange()[1];
                    value = ((unsigned char) (l << (7 - t))) >> (7 - t + s);
                    if (cur.rangeIn()[0] < 0) {
                        value = (signed char)(value);
                    }
                }
#endif
                    unsigned int low_byte = 0;
                    int full = 0;
                    if (cur.highByte() != -1) {
                        unsigned int high_byte = 0;
                        high_byte = buf[j]->Data[cur.highByte()];
                        high_byte <<= (7 - cur.highByteRange()[1]);
                        high_byte >>= (7 - cur.highByteRange()[1] + cur.highByteRange()[0]);
                        high_byte <<= (cur.lowByteRange()[1] - cur.lowByteRange()[0] + 1);

                        low_byte = buf[j]->Data[cur.lowByte()];
                        low_byte <<= (7 - cur.lowByteRange()[1]);
                        low_byte >>= (7 - cur.lowByteRange()[1] + cur.lowByteRange()[0]);

                        full = high_byte + low_byte;
                        if (cur.rangeIn()[0] < 0) {
                            full = (signed short) (full);
                        }
                    } else {
                        low_byte = buf[j]->Data[cur.lowByte()];
                        low_byte <<= (7 - cur.lowByteRange()[1]);
                        low_byte >>= (7 - cur.lowByteRange()[1] + cur.lowByteRange()[0]);
                        full = low_byte;
                        if (cur.rangeIn()[0] < 0) {
                            full = (signed char) (full);
                        }
                    }
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
                    tr.push(Tribe::FakeByPrevious, tr.last());
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
    _cmd = Re::NoCommand;
    _tribe->reset();
    start(QThread::HighestPriority);
    _status = Re::Running;
}

void Transform::stop(const QString &file)
{
    _cmd = Re::CommandStop;
    while (isRunning()) {}
    if (!file.isEmpty()) {
        QFile f(file);
        _file.dumpCurveRecord(f, *_tribe);
    }
    _status = Re::Stop;
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

