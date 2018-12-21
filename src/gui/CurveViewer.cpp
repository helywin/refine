//
// Created by jiang.wenqiang on 2018/10/24.
//

#include <QtCore/QDebug>
#include <QtCore/QtMath>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include "CurveViewer.hpp"
#include "SketchY.hpp"
#include "SketchX.hpp"
#include "SketchXTop.hpp"
#include "Revolve.hpp"
#include "Tribe.hpp"

CurveViewer::CurveViewer(QWidget *parent, Revolve *revolve, Message *message) :
        QWidget(parent),
        Message(message),
        _sketch_y(new SketchY(this, revolve, this)),
        _sketch_xtop(new SketchXTop(this, revolve, this)),
        _sketch_x(new SketchX(this, revolve, this)),
        _sketch(new Sketch(this, revolve, this)),
        _tribe(&revolve->tribe()),
        _zoom_x(true),
        _zoom_y(false),
        _msec(10),
        _status(Re::Stop),
        _command(Re::NoCommand),
        _h_scroll_pressed(false),
        _is_rolling(false)
{
    setup();
}

void CurveViewer::setup()
{
    _layout = new QGridLayout(this);
    _widget_sketch = new QWidget(this);
    _layout_sketch = new QGridLayout(_widget_sketch);
    _layout_sketch->setContentsMargins(0, 0, 0, 0);
    _widget_sketch->setLayout(_layout_sketch);
    _layout_sketch->addWidget(_sketch_y, 0, 0, 3, 1);
    _layout_sketch->addWidget(_sketch_xtop, 0, 1, 1, 1);
    _layout_sketch->addWidget(_sketch, 1, 1, 1, 1);
    _layout_sketch->addWidget(_sketch_x, 2, 1, 1, 1);
    auto right_blank = new QOpenGLWidget(this);
    right_blank->setFixedWidth(15);
    _layout_sketch->addWidget(right_blank, 0, 2, 3, 1);
    _layout_sketch->setSpacing(0);
    setLayout(_layout);
    _h_scroll = new QScrollBar(Qt::Orientation::Horizontal, this);
    _v_scroll = new QScrollBar(Qt::Orientation::Vertical, this);
    QFile file(":res/qss/scrollbar.css");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (file.isOpen()) {
        QTextStream stream(&file);
        QString scrollbar_css = stream.readAll();
//        qDebug() << scrollbar_css;
        _h_scroll->setStyleSheet(scrollbar_css);
        _v_scroll->setStyleSheet(scrollbar_css);
    }
    file.close();
    _layout->addWidget(_widget_sketch, 0, 0);
    _layout->addWidget(_h_scroll, 1, 0);
    _layout->addWidget(_v_scroll, 0, 1);
    _layout->setMargin(0);
    _layout->setContentsMargins(0, 0, 0, 0);
    _h_scroll->setMaximum(0);
    _v_scroll->setMaximum(0);
    _timer.setInterval(_msec);

    _menu = new QMenu();


    connect(_h_scroll, &QScrollBar::valueChanged,
            this, &CurveViewer::hScrollChanged, Qt::DirectConnection);
    connect(_v_scroll, &QScrollBar::valueChanged,
            this, &CurveViewer::vScrollChanged, Qt::DirectConnection);
    connect(_sketch, &Sketch::zoomDefault, this, &CurveViewer::zoomDefault);
    connect(_sketch, &Sketch::zoomMinimum, this, &CurveViewer::zoomMinimum);
    connect(_sketch, &Sketch::zoomPlus, this, &CurveViewer::zoomPlus);
    connect(_sketch, &Sketch::zoomMinus, this, &CurveViewer::zoomMinus);
    connect(_sketch, &Sketch::parallelMove, this, &CurveViewer::parallelMove);
    connect(_sketch, &Sketch::vernierMove, _sketch_xtop, &SketchXTop::vernierTextMove);
    connect(&_timer, &QTimer::timeout, this, &CurveViewer::rollViewer);
    connect(_h_scroll, &QScrollBar::sliderPressed, this, [=]() { _h_scroll_pressed = true; });
    connect(_h_scroll, &QScrollBar::sliderReleased, this, [=]() { _h_scroll_pressed = false; });
//    connect(_sketch, &Sketch::rightMouseBottonRelease, this, &CurveViewer::showContextMenu);
}

void CurveViewer::hScrollChanged(int value)
{
    if (value < 0) {
        value = 0;
    } else if (value > _h_scroll->maximum()) {
        value = _h_scroll->maximum();
    }
    _sketch->setXStart(value);
    _sketch_x->setXStart(value);
    _sketch->update();
    _sketch_x->update();
}

void CurveViewer::vScrollChanged(int value)
{
    double rate = (_v_scroll->maximum() - value) / Sketch::Y_POINTS;
    _sketch->setYStart(rate);
    _sketch_y->setYStart(rate);
    _sketch->update();
    _sketch_y->update();
}

void CurveViewer::resetHScroll(int len, bool reset)
{
    _h_scroll->setMinimum(0);
    int points = _sketch->xPoints();
    if (len <= points) {
        _h_scroll->setMaximum(0);
    } else {
        _h_scroll->setMaximum(len - points);
    }
    _h_scroll->setPageStep(points);
    if (reset) {
        _h_scroll->setValue(0);
        update();
    } else {
        _h_scroll->setValue(_h_scroll->maximum());
    }
    emitMessage(Re::Debug, tr("重设滚动条大小: %1").arg(_h_scroll->maximum()));
}

void CurveViewer::resetVScroll(int len)
{
    _v_scroll->setMinimum(0);
    _v_scroll->setMaximum(int(Sketch::Y_POINTS));
}

//void CurveViewer::zoomPlus()
//{
//    if (_zoom_x) {
//        double rate = _sketch->xRate() / 1.5;
//        if (rate < 1e-2) {
//            rate = 1e-2;
//        }
//        _sketch->setXRate(rate);
//        _h_scroll->setMaximum(_tribe->len() - _sketch->xPoints());
//        _h_scroll->setPageStep(_sketch->xPoints());
//        _sketch_x->setPoints(_sketch->xPoints());
//        _sketch_x->update();
//    }
//    if (_zoom_y) {
//        double rate = _sketch->yRate() / 1.5;
//        if (rate < 1e-2) {
//            rate = 1e-2;
//        }
////        bool first = _sketch->yRate() == 1.0;
////        int last_v;
////        if (first) {
////            last_v = int(Sketch::Y_POINTS * (1 - rate));
////        } else {
////        }
//        int last_v = _v_scroll->maximum() - _v_scroll->value();
//        _v_scroll->setMaximum(int(Sketch::Y_POINTS * (1 - rate)));
//        _v_scroll->setPageStep(int(Sketch::Y_POINTS * rate));
//        _sketch->setYRate(rate);
//        _v_scroll->setValue(int(_v_scroll->maximum() - last_v));
//        _sketch_y->setYStart(_sketch->yStart());
//        _sketch_y->setYRate(_sketch->yRate());
//        _sketch_y->update();
//    }
//    _sketch->update();
//}

void CurveViewer::zoomDefault()
{
    if (_zoom_x) {
        zoomXDefault();
    }
    if (_zoom_y) {
        zoomYDefault();
    }
    _sketch->update();
}

void CurveViewer::zoomMinimum()
{
    if (_zoom_x) {
        zoomXMinimum();
    }
    if (_zoom_y) {
        zoomYMinimum();
    }
    _sketch->update();
}

void CurveViewer::setXZoom(bool flag)
{
    _zoom_x = flag;
    _sketch->setXZoom(flag);
}

void CurveViewer::setYZoom(bool flag)
{
    _zoom_y = flag;
    _sketch->setYZoom(flag);
}

void CurveViewer::zoomPlus(double x_rate, double x_start, double y_rate, double y_start)
{
    if (_zoom_x) {
        zoomX(x_rate, x_start);
    }
    if (_zoom_y) {
        zoomY(y_rate, y_start);
    }
    _sketch->update();
}

void CurveViewer::zoomMinus(double x_rate, double x_start,
                            double y_rate, double y_start, Sketch::ZoomEdges edge)
{
    if (_zoom_x) {
        if ((edge & Sketch::EdgeLeft) && (edge & Sketch::EdgeRight)) {
            emitMessage(Re::Debug, "x轴缩小 EdgeX");
            zoomXMinimum();
        } else if (edge & Sketch::EdgeLeft) {
            emitMessage(Re::Debug, "x轴缩小 EdgeLeft");
            zoomXMinusEdgeLeft(x_rate);
        } else if (edge & Sketch::EdgeRight) {
            emitMessage(Re::Debug, "x轴缩小 EdgeRight");
            zoomXMinusEdgeRight(x_rate);
        } else {
            emitMessage(Re::Debug, "x轴缩小 free");
            zoomX(x_rate, x_start);
        }
    }
    if (_zoom_y) {
        if ((edge & Sketch::EdgeTop) && (edge & Sketch::EdgeBottom)) {
            zoomYMinimum();
        } else if (edge & Sketch::EdgeBottom) {
            zoomYMinusEdgeBottom(y_rate);
            emitMessage(Re::Debug, "y轴缩小 EdgeBottom");
        } else if (edge & Sketch::EdgeTop) {
            zoomYMinusEdgeTop(y_rate);
            emitMessage(Re::Debug, "y轴缩小 EdgeTop");
        } else {
            zoomY(y_rate, y_start);
            emitMessage(Re::Debug, "y轴缩小 free");
        }
    }
    _sketch->update();
}

void CurveViewer::parallelMove(double delta_x, double delta_y)
{
    int start_pos_x = _sketch->xStart() + qRound(delta_x * _sketch->xPointsF());
    if (start_pos_x < 0) {
        start_pos_x = 0;
    } else if (start_pos_x > _tribe->len() - _sketch->xPointsF()) {
        start_pos_x = _tribe->len() - _sketch->xPoints();
    }
    Q_ASSERT(start_pos_x >= 0);
    _sketch->setXStart(start_pos_x);
    _h_scroll->setValue(start_pos_x);
    _sketch_x->setXStart(start_pos_x);
    _sketch_x->update();

    double y_rate = _sketch->yRate();
    double start_pos_y = _sketch->yStart() + delta_y * y_rate;
    if (start_pos_y < 0) {
        start_pos_y = 0;
    } else if (start_pos_y > 1 - y_rate) {
        start_pos_y = 1 - y_rate;
    }
    _sketch->setYStart(start_pos_y);
    _v_scroll->setValue(int(_v_scroll->maximum() * (1 - start_pos_y / (1 - y_rate))));
    _sketch_y->setYStart(start_pos_y);
    _sketch_y->update();

    _sketch->update();
}

//void CurveViewer::zoomPlusFixed()
//{
//
//}
//
//void CurveViewer::zoomMinusFixed()
//{
//
//}

void CurveViewer::zoomX(double rate, double start)
{
    int start_pos = _sketch->xStart() + qFloor(_sketch->xPointsF() * start);
    Q_ASSERT(start_pos >= 0);
    _sketch->setXStart(start_pos);
    _sketch->setXRate(rate * _sketch->xRate());
    int max = _tribe->len() - _sketch->xPoints();
    if (max < 0) {
        qDebug() << "CurveViewer::zoomX max: " << max
                 << "_tribe->len(): " << _tribe->len()
                 << "_sketch->xPoints(): " << _sketch->xPoints()
                 << "_sketch->xRate()" << _sketch->xRate();
        max = 0;
    }
    _h_scroll->setMaximum(max);
    _h_scroll->setPageStep(_sketch->xPoints());
    _h_scroll->setValue(start_pos);
    _sketch_x->setPoints(_sketch->xPoints());
    _sketch_x->setXStart(start_pos);
    _sketch_x->update();
}

void CurveViewer::zoomY(double rate, double start)
{
    double last_rate = _sketch->yRate();
    double last_start = _sketch->yStart();
    double y_rate = last_rate * rate;
    double y_start = last_start + last_rate * start;
    _v_scroll->setMaximum(int(Sketch::Y_POINTS * (1 - y_rate)));
    _v_scroll->setPageStep(int(Sketch::Y_POINTS * y_rate));
    _sketch->setYStart(y_start);
    _sketch->setYRate(y_rate);
    _v_scroll->setValue(int(_v_scroll->maximum() * (1 - y_start / (1 - y_rate))));
    _sketch_y->setYStart(y_start);
    _sketch_y->setYRate(y_rate);
    _sketch_y->update();
}

void CurveViewer::zoomXMinusEdgeLeft(double rate)
{
    int start_pos = 0;
    Q_ASSERT(start_pos >= 0);
    _sketch->setXStart(start_pos);
    _sketch->setXRate(rate * _sketch->xRate());
    _h_scroll->setMaximum(_tribe->len() - _sketch->xPoints());
    _h_scroll->setPageStep(_sketch->xPoints());
    _h_scroll->setValue(start_pos);
    _sketch_x->setPoints(_sketch->xPoints());
    _sketch_x->setXStart(start_pos);
    _sketch_x->update();
}

void CurveViewer::zoomXMinusEdgeRight(double rate)
{
    int start_pos = qFloor(_tribe->len() - _sketch->xPointsF() * rate);
    Q_ASSERT(start_pos >= 0);
    _sketch->setXStart(start_pos);
    _sketch->setXRate(rate * _sketch->xRate());
    _h_scroll->setMaximum(_tribe->len() - _sketch->xPoints());
    _h_scroll->setPageStep(_sketch->xPoints());
    _h_scroll->setValue(start_pos);
    _sketch_x->setPoints(_sketch->xPoints());
    _sketch_x->setXStart(start_pos);
    _sketch_x->update();
}

void CurveViewer::zoomYMinusEdgeBottom(double rate)
{
    double last_rate = _sketch->yRate();
    double y_rate = last_rate * rate;
    double y_start = 0;
    _v_scroll->setMaximum(int(Sketch::Y_POINTS * (1 - y_rate)));
    _v_scroll->setPageStep(int(Sketch::Y_POINTS * y_rate));
    _sketch->setYStart(y_start);
    _sketch->setYRate(y_rate);
    _v_scroll->setValue(int(_v_scroll->maximum() * (1 - y_start / (1 - y_rate))));
    _sketch_y->setYStart(y_start);
    _sketch_y->setYRate(y_rate);
    _sketch_y->update();
}

void CurveViewer::zoomYMinusEdgeTop(double rate)
{
    double last_rate = _sketch->yRate();
    double y_rate = last_rate * rate;
    double y_start = 1 - rate * _sketch->yRate();
    _v_scroll->setMaximum(int(Sketch::Y_POINTS * (1 - y_rate)));
    _v_scroll->setPageStep(int(Sketch::Y_POINTS * y_rate));
    _sketch->setYStart(y_start);
    _sketch->setYRate(y_rate);
    _v_scroll->setValue(int(_v_scroll->maximum() * (1 - y_start / (1 - y_rate))));
    _sketch_y->setYStart(y_start);
    _sketch_y->setYRate(y_rate);
    _sketch_y->update();
}

void CurveViewer::zoomXDefault()
{
    if (_tribe->len() < Sketch::X_POINTS) {
        return;
    }
    _sketch->setXRate(1);
    int max = _tribe->len() - _sketch->xPoints();
    if (_h_scroll->value() > max) {
        _h_scroll->setValue(max);
        Q_ASSERT(max >= 0);
        _sketch->setXStart(max);
    }
    _h_scroll->setMaximum(max);
    _h_scroll->setPageStep(_sketch->xPoints());
    _sketch_x->setPoints(_sketch->xPoints());
    _sketch_x->update();
}

void CurveViewer::zoomYDefault()
{
    _sketch->setYRate(1.0);
    _v_scroll->setPageStep(int(Sketch::Y_POINTS));
    _v_scroll->setValue(0);
    _v_scroll->setMaximum(0);
    _sketch_y->setYRate(1.0);
    _sketch_y->setYStart(0.0);
    _sketch_y->update();
}

void CurveViewer::zoomXMinimum()
{
    _sketch->setXRate(_sketch->maximumXRate());
    _sketch->setXStart(0);
    _h_scroll->setMaximum(0);
    _h_scroll->setValue(0);
    _h_scroll->setPageStep(_sketch->xPoints());
    _sketch_x->setXStart(0);
    _sketch_x->setPoints(_sketch->xPoints());
    _sketch_x->update();
}

void CurveViewer::zoomYMinimum()
{
    _sketch->setYRate(1.0);
    _sketch->setYStart(0.0);
    _v_scroll->setPageStep(int(Sketch::Y_POINTS));
    _v_scroll->setMaximum(0);
    _v_scroll->setValue(0);
    _sketch_y->setYRate(1.0);
    _sketch_y->setYStart(0.0);
    _sketch_y->update();
}

void CurveViewer::start()
{
    _sketch->setDisplayMode(Sketch::Rolling);
    _timer.start();
}

void CurveViewer::pause()
{
    _timer.stop();
    while (_is_rolling) {}  //防止冲突
    _sketch->setDisplayMode(Sketch::Free);
    regen(false);
}

void CurveViewer::resume()
{
    _sketch->setDisplayMode(Sketch::Rolling);
    _timer.start();
}

void CurveViewer::stop()
{
    _timer.stop();
    while (_is_rolling) {}  //防止冲突
    _sketch->setDisplayMode(Sketch::Free);
    regen(true);
}

void CurveViewer::rollViewer()
{
    _is_rolling = true;
    if (_h_scroll_pressed) {
        return;
    }
    int start = _tribe->len() - _sketch->xPoints();
    if (start < 0) {
        start = 0;
    }
    _h_scroll->setMaximum(start);
    _h_scroll->setValue(start);
    Q_ASSERT(start >= 0);
    _sketch->setXStart(start);
    _sketch_x->setXStart(start);
    _sketch->updateVernier();
    _sketch->update();
    _sketch_x->update();
    _sketch_xtop->update();
    _is_rolling = false;
}

void CurveViewer::regen(bool zoom)
{
    int x_start;
    double x_rate;
    int h_scroll;
    if (zoom) {
        x_start = 0;
        x_rate = _tribe->len() / (double) Sketch::X_POINTS;
        h_scroll = 0;
    } else {
        x_start = _sketch->xStart();
        x_rate = _sketch->xRate();
        h_scroll = _h_scroll->value();
    }
    _sketch->setDisplayMode(Sketch::Free);
    Q_ASSERT(x_start >= 0);
    _sketch->setXStart(x_start);
    _sketch->setXRate(x_rate);
    _sketch_x->setXStart(x_start);
    _sketch_x->setPoints(_sketch->xPoints());
    if (_tribe->len() <= _sketch->xPointsF()) {
        _h_scroll->setMaximum(0);
    } else {
        _h_scroll->setMaximum(qFloor(_tribe->len() - _sketch->xPointsF()));
    }
    _h_scroll->setMaximum(0);
    _h_scroll->setValue(h_scroll);
    _h_scroll->setPageStep(_sketch->xPoints());

    double y_start;
    double y_rate;
    if (zoom) {
        y_start = 0;
        y_rate = 1;
        _v_scroll->setMaximum(0);
        _v_scroll->setValue(0);
    } else {
        y_start = _sketch->yStart();
        y_rate = _sketch->yRate();
    }
    _sketch->setYStart(y_start);
    _sketch_y->setYStart(y_start);
    _sketch->setYRate(y_rate);
    _sketch_y->setYRate(y_rate);

    _sketch_x->update();
    _sketch_y->update();
    _sketch_xtop->update();
    _sketch->updateVernier();
    _sketch->update();
}

void CurveViewer::mouseReleaseEvent(QMouseEvent *event)
{
//    emitMessage(Re::Debug,
//                QString("botton:%1, bottons:%2").arg(event->button()).arg(event->buttons()));
    if (event->button() == Qt::RightButton) {
        _menu->exec(event->globalPos());
        event->accept();
    }
}

void CurveViewer::finishMenuSet()
{
    _menu->addAction(_menu_start);
    _menu->addAction(_menu_pause);
    _menu->addAction(_menu_resume);
    _menu->addAction(_menu_stop);
//    initMenu(_menu_start, tr("开始采集"), _menu);
//    _menu_start->setIcon(QIcon(":res/icons/start.png"));
//    initMenu(_menu_pause, tr("暂停采集"), _menu);
//    _menu_pause->setIcon(QIcon(":res/icons/pause.png"));
//    initMenu(_menu_resume, tr("继续采集"), _menu);
//    _menu_resume->setIcon(QIcon(":res/icons/resume.png"));
//    initMenu(_menu_stop, tr("停止采集"), _menu);
//    _menu_stop->setIcon(QIcon(":res/icons/stop.png"));
    _menu->addSeparator();
    _menu->addAction(_menu_load);
    _menu->addAction(_menu_save);
//    _menu->addAction(_menu_current);

//    initMenu(_menu_load, tr("加载数据..."), _menu);
//    _menu_load->setIcon(QIcon(":res/icons/open.png"));
//    initMenu(_menu_save, tr("保存数据..."), _menu);
//    _menu_save->setIcon(QIcon(":res/icons/save.png"));
    initMenu(_menu_current, tr("保存当前显示数据..."), _menu);
    _menu_current->setIcon(QIcon(":res/icons/export.png"));
    _menu->addSeparator();
    initMenu(_menu_copy_img, tr("复制图像"), _menu);
    initMenu(_menu_save_img, tr("保存图像"), _menu);
    initMenu(_menu_clip_img, tr("截取图像"), _menu);
    _menu->addSeparator();
    initMenu(_menu_vernier, tr("显示游标"), _menu);
    _menu_vernier->setCheckable(true);
    _menu_vernier->setChecked(true);
    initMenu(_menu_pattern, tr("显示区间"), _menu);
    _menu_pattern->setCheckable(true);
    initMenu(_menu_marks, tr("显示标记"), _menu);
    _menu_marks->setCheckable(true);
    _menu_marks->setChecked(true);
    _menu->addSeparator();
    _menu->addAction(_menu_settings);
//    initMenu(_menu_settings, tr("设置"), _menu);
//    _menu_settings->setIcon(QIcon(":res/icons/settings.png"));
    setContextMenuPolicy(Qt::CustomContextMenu);
    _sketch->setMenu(_menu);
}


