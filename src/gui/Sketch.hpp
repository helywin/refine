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

    enum Movement
    {
        MoveNone,
        MoveVernier,
        MoveZoomRect
    };

    enum ZoomEdge
    {
        NoEdge = 0x00,
        EdgeLeft = 0x01,
        EdgeRight = 0x02,
        EdgeX = EdgeLeft | EdgeRight,   //运算的或相当于意思上的与
        EdgeTop = 0x04,
        EdgeBottom = 0x08,
        EdgeY = EdgeTop | EdgeBottom
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
        double pos;                 //relative pos
        int start;                  //absolute pos
    };

    const static double X_LEFT;
    const static int X_POINTS;

    const static double Y_BOTTOM;
    const static double Y_POINTS;

private:
    Tribe *_tribe;
    DisplayMode _mode;

    //! \brief OpenGL坐标范围
    int _x_start;           //点的索引位置
    double _x_rate;         //0~无穷
    double _x_sec;          //浮点
    int _x_end;
    double _y_start;        //都是取0-1内的浮点
    double _y_rate;         //都是取0-1内的浮点

    QColor _background_color;
    QColor _grid_color;

    int _current_index;
    bool _smooth;
    bool _vernier_visible;
    bool _vernier_fix;
    bool _vernier_back;
    QVector<Vernier> _verniers;
    QVector<Pattern> _patterns;
    int _y_graduate_num;
    int _x_graduate_num;

    Movement _movement;
    bool _zoom_x;
    bool _zoom_y;

    QRect _zoom_rect;
    bool _plot_zoom_rect;
    bool _zoom_finish;

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

    inline double xRate() const { return _x_rate; }

    inline void setXRate(double rate) { _x_rate = rate; }

    inline void setXStart(int start) { _x_start = start; }

    inline double yStart() const { return _y_start; }

    inline double yPoints() const { return Y_POINTS * _y_rate; }

    inline void setYStart(double start) { _y_start = start; }

    inline double yRate() const { return _y_rate; }

    inline void setYRate(double rate) { _y_rate = rate; }

    inline void calculateXEnd()
    {
        _x_end = _x_start + xPoints();
        if (_tribe->len() < _x_end) {
            _x_end = _x_start + _tribe->len();
        }
    }

    inline float yToGl(float y, const Tribe::Style &style) const
    {
        auto y0 = static_cast<float>
        ((y - style.rangeOut()[0]) * (Sketch::Y_POINTS) /
         (style.rangeOut()[1] - style.rangeOut()[0]) + 0);
        return float((y0 - Y_POINTS * _y_start) / _y_rate);
    }

    inline void setXZoom(bool flag) { _zoom_x = flag; }

    inline void setYZoom(bool flag) { _zoom_y = flag; }

    inline double xZoomPos() const { return _verniers[0].pos / X_POINTS; }

    double yZoomPos() const;

    inline void setBackgroundColor(const QColor &color)
    {
        _background_color = color;
        glClearColor((float) _background_color.redF(), (float) _background_color.greenF(),
                     (float) _background_color.blueF(), (float) _background_color.alphaF());
        update();
    }

    inline void setGridColor(const QColor &color)
    {
        _grid_color = color;
        update();
    }

    void zoomPlusFixed();

    void zoomMinusFixed();

    void zoomPlusRect();

    bool xZoomPlusLimit() const;

    bool yZoomPlusLimit() const;

    inline double maximumXRate() const { return _tribe->len() / (double) Sketch::X_POINTS; }

    inline double maximumYRate() const { return 1; }

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

    void plotZoomRect();

    void drawGlString(double x0, double y0, const QString &str,
                      const QColor &color, const QFont &font);

    void drawQtString(int x0, int y0, const QString &str, const QColor &color,
                      const QFont &font);

    void pointGlToQt(double x0, double y0, int &x1, int &y1);

    void pointQtToGl(int x0, int y0, double &x1, double &y1);

    int xGlToQt(double x) const;
    double xQtToGl(int x) const;

    int yGlToQt(double y) const;
    double yQtToGl(int y) const;

    void drawFocusSign();

    bool isMouseOnDragItem(int x);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
signals:
    void scrollMove(int angle);

    void zoomPlus(double x_rate, double x_start, double y_rate, double y_start);

    void zoomMinus(double x_rate, double x_start,
                   double y_rate, double y_start, int edge);

    void zoomDefault();

    void zoomMinimum();

    void vernierMove(int pos, double time);

};


#endif //REFINE_SKETCH_HPP
