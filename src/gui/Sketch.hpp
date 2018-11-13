//
// Created by jiang.wenqiang on 2018/9/28.
//

#ifndef REFINE_SKETCH_HPP
#define REFINE_SKETCH_HPP

#include <QtCore/QTimer>
#include <QtGui/QWheelEvent>
#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLFunctions>
#include <QtWidgets/QScrollBar>
#include "Tribe.hpp"
#include "Curve.hpp"
#include "Message.hpp"

class Revolve;
/*!
 * @brief 显示曲线的控件类
 */

#define X_LEFT 0.0
#define X_RIGHT 2000.0
#define Y_BOTTOM 0.0
#define Y_TOP 4096.0
#define X_L_BLANK 50
#define X_R_BLANK_RATE 0.02
#define Y_BLANK_RATE 0.1

class Sketch : public QOpenGLWidget, protected QOpenGLFunctions, public Message
{
Q_OBJECT
public:
    enum DisplayMode
    {
        Rolling,
        Free
    };
private:
    Tribe *_tribe;
    QTimer _timer;
    int _msec;
    QScrollBar *_h_scroll;
    DisplayMode _mode;
    int _x_pos;
    double _y_pos;
    double _x_rate;
    double _y_rate;
    int _current_index;
    int _init_w;
    double _left_axis_width;
    bool _smooth;

public:
    explicit Sketch(QWidget *parent, Revolve *revolve,
            Message *message = nullptr);

    inline void setScroll(QScrollBar *scroller) { _h_scroll = scroller; }

    void setSmooth(bool enable);

    void start(int msec = 10);
    void pause();
    void resume();
    void stop();

    void initData();

    inline void setMsec(int msec)
    {
        _msec = msec;
        _timer.setInterval(_msec);
    }

    inline void setCurrentIndex(int index) { _current_index = index; }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:

    inline float genY(float y, const Tribe::Style &style)
    {
        return (float) ((y - style.rangeOut()[0]) *
                        (Y_TOP) / (style.rangeOut()[1] - style.rangeOut()[0])
                        + Y_BOTTOM);
    }

    void plotXAxis();

    void plotYAxis();

    void plotVernier();

    void plotCurves();
};


#endif //REFINE_SKETCH_HPP
