//
// Created by jiang.wenqiang on 2018/9/28.
//

#ifndef REFINE_SKETCH_HPP
#define REFINE_SKETCH_HPP

#include <QtCore/QTimer>
#include <QtGui/QWheelEvent>
#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLExtraFunctions>
#include <QtGui/QOpenGLBuffer>
#include <QtWidgets/QScrollBar>
#include "Tribe.hpp"
#include "Combine.hpp"
#include "Curve.hpp"
#include "Message.hpp"

class Revolve;
/*!
 * @brief 显示曲线的控件类
 */

#define X_LEFT 0.0
//! \brief 默认的OpenGL横坐标宽度
#define X_POINTS 2000.0
#define Y_BOTTOM 0.0
//! \brief 默认的OpenGL纵坐标宽度
#define Y_POINTS 4096.0
#define X_L_BLANK 50
#define X_R_BLANK_RATE 0.02
#define Y_BLANK_RATE 0.1

class Sketch :
        public QOpenGLWidget,
        protected QOpenGLExtraFunctions,
        public Message
{
Q_OBJECT
public:
    /*!
     * @brief 显示模式
     */
    enum DisplayMode
    {
        Waiting,
        Rolling,
        Free,
        Empty
    };

    /*!
     * @brief 区间数据
     */
    struct Pattern
    {
        int index;
        Tribe::Slice slice;
    };

    /*!
     * @brief 游标数据
     */
    struct Vernier
    {
        int index;
        double pos;
    };

    const double Y_MIN = 0;
    const double Y_MAX = 4096;

private:
    Tribe *_tribe;
    Combine *_combine;
    QTimer _timer;
    DisplayMode _mode;
    int _msec;
    QScrollBar *_h_scroll;

    //! \brief OpenGL坐标范围
    int _points;
    double _x_start;
    double _x_end;
    double _y_start;
    double _y_end;

    int _x_pos;
    double _y_pos;
    double _x_rate;
    double _y_rate;
    int _current_index;
    bool _smooth;
    bool _vernier;
    int _graduate_num;
    int _vernier_pos;

    GLuint *_curve_buffers;
    GLuint *_vaos;
    GLsizei _buffer_size;   //also vao_nums


public:
    explicit Sketch(QWidget *parent, Revolve *revolve,
                    Message *message = nullptr);

    inline void setScroll(QScrollBar *scroll) { _h_scroll = scroll; }

    void setSmooth(bool enable);

    void start(int msec = 20);
    void pause();
    void resume();
    void stop();

    void init();

    inline void setMsec(int msec)
    {
        _msec = msec;
        _timer.setInterval(_msec);
    }

    inline void setCurrentIndex(int index) { _current_index = index; }

public slots:

    inline void setGraduateNum(int num) { _graduate_num = num; }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:

    inline float genY(float y, const Tribe::Style &style)
    {
        return (float) ((y - style.rangeOut()[0]) *
                        (Y_POINTS) / (style.rangeOut()[1] - style.rangeOut()[0])
                        + Y_BOTTOM);
    }

    void plotXAxis();

    void plotYGrid();

    void plotVernier();

    void plotCurves();

    void drawGlString(double x0, double y0, const QString &str,
                      const QColor &color, const QFont &font);

    void drawQtString(int x0, int y0, const QString &str, const QColor &color,
                      const QFont &font);

    void pointGlToQt(double x0, double y0, int &x1, int &y1);

    void pointQtToGl(int x0, int y0, double &x1, double &y1);

    int xGlToQt(double x);
    double xQtToGl(int x);

    int yGlToQt(double y);
    double yQtToGl(int y);

    void drawFocusSign();

    void scrollMove(int angle);

    inline void setXRate(double x_rate)
    {
        _x_rate = x_rate;
        update();
    }

    inline void setYRate(double y_rate)
    {
        _y_rate = y_rate;
        update();
    }

protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};


#endif //REFINE_SKETCH_HPP
