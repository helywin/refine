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
        Tribe::Slice slice;     //absolute pos
    };

    /*!
     * @brief 游标数据
     */
    struct Vernier
    {
        int index;
        int pos;                //relative pos
        int start;              //absolute pos
    };

    const static double X_LEFT;
    const static int X_POINTS;

    const static double Y_BOTTOM;
    const static double Y_POINTS;

private:
    Tribe *_tribe;
    Combine *_combine;
    DisplayMode _mode;

    //! \brief OpenGL坐标范围
    int _x_start;
    double _x_rate;
    int _x_end;
    double _y_start;
    double _y_rate;

    int _current_index;
    bool _smooth;
    bool _vernier_visible;
    bool _vernier_fix;
    QVector<Vernier> _verniers;
    QVector<Pattern> _patterns;
    int _graduate_num;

#ifdef VERTEX
    GLuint *_curve_buffers;
    GLuint *_vaos;
    GLsizei _buffer_size;   //also vao_nums
#endif

public:
    explicit Sketch(QWidget *parent, Revolve *revolve,
                    Message *message = nullptr);

    void setSmooth(bool enable);

    void init();

    inline void setCurrentIndex(int index) { _current_index = index; }

    inline int xStart() const { return _x_start; }

    inline int xPoints() const { return qRound(X_POINTS * _x_rate); }

    inline void setXRate(double rate) { _x_rate = rate; }

    inline void setXStart(int start) { _x_start = start; }

    inline double yStart() const { return _y_start; }

    inline double yPoints() const { return Y_POINTS * _y_rate; }

    inline void setYStart(double start) { _y_start = start; }

    inline void setYRate(double rate) { _y_rate = rate; }

    inline void calculateXEnd()
    {
        _x_end = _x_start + xPoints();
        if (_tribe->len() < _x_end) {
            _x_end = _x_start + _tribe->len();
        }
    }


public slots:
    inline void setGraduateNum(int num) { _graduate_num = num; }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void plotXGrid();

    void plotYGrid();

    void plotVerniers();

    void plotPatterns();

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

protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void scrollMove(int angle);

};


#endif //REFINE_SKETCH_HPP
