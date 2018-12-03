//
// Created by jiang.wenqiang on 2018/9/28.
//

#include <cmath>
#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <QtCore/QVector>
#include "Sketch.hpp"
#include "Revolve.hpp"

Sketch::Sketch(QWidget *parent, Revolve *revolve, Message *message) :
        QOpenGLWidget(parent),
        Message(message),
        _tribe(&revolve->tribe()),
        _combine(&revolve->combine()),
        _msec(10),
        _h_scroll(nullptr),
        _points(2000),
        _mode(Waiting),
        _x_pos(0),
        _y_pos(0),
        _x_rate(1),
        _y_rate(1),
        _x_start(0),
        _x_end(20),
        _current_index(-1),
        _smooth(true),
        _vernier(false),
        _vernier_pos(int(X_POINTS * _x_rate / 2)),
        _buffer_size(-1)
{
    _timer.setInterval(_msec);
    setMinimumWidth(400);
    connect(&_timer, &QTimer::timeout,
            this, static_cast<void (Sketch::*)(void)>(&Sketch::update),
            Qt::DirectConnection);
    setFocusPolicy(Qt::StrongFocus);
//    setMouseTracking(true);
}

void Sketch::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
//    glEnable(GL_DEPTH_TEST);
//    setAutoBufferSwap(true);
    setSmooth(true);
}

void Sketch::resizeGL(int w, int h)
{

    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(_x_start, _x_end, Y_MIN, Y_MIN + Y_MAX, 0, 100);

}

void Sketch::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
//    plotXAxis();
    plotCurves();
//    plotVernier();
//    glFlush();
}

void Sketch::setSmooth(bool enable)
{
    _smooth = enable;
    QOpenGLWidget::repaint();
}

void Sketch::start(int msec)
{
    _mode = Rolling;
    _timer.start();     //采用低优先级的定时器，减少开支
    _h_scroll->setDisabled(true);
}

void Sketch::pause()
{
    _timer.stop();
    _h_scroll->setDisabled(false);
    init();
}

void Sketch::resume()
{
    _timer.start();
    _h_scroll->setDisabled(true);
}

void Sketch::stop()
{
    _timer.stop();
    _mode = Free;
    _h_scroll->setDisabled(false);
    init();
}

void Sketch::init()
{
    _points = _tribe->len();
#ifdef VERTEX
    if (_buffer_size > 0) {
        glDeleteBuffers(_buffer_size, _curve_buffers);
        glDeleteVertexArrays(_buffer_size, _vaos);
        delete[]_vaos;
        delete[]_curve_buffers;
        qDebug() << "Sketch::init() delete";
    }

    _buffer_size = _tribe->size();
    qDebug() << "Sketch::init() size: " << _buffer_size;
    if (_buffer_size <= 0) {
        return;
    }
    _curve_buffers = new GLuint[_buffer_size];
    _vaos = new GLuint[_buffer_size];

    glGenBuffers(_buffer_size, _curve_buffers);
    glGenVertexArrays(_buffer_size, _vaos);

    qopengl_GLsizeiptr size = sizeof(float) * _combine->len();
    qDebug() << "Sketch::init() vsize: " << size;
    for (GLsizei i = 0; i < _buffer_size; ++i) {
        glBindBuffer(GL_ARRAY_BUFFER, _curve_buffers[i]);
        glBufferData(GL_ARRAY_BUFFER, size, (*_combine)[i].data().data(), GL_DYNAMIC_DRAW);

        glBindVertexArray(_vaos[i]);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);
    }
#endif
    _mode = Free;
    glOrtho(_x_start, _x_end, Y_MIN, Y_MIN + Y_MAX, 0, 100);
}


/*
void Sketch::plotCurves()
{
    if (_smooth) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    } else {
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
    }
    int start_pos = 0;
    int len = 0;
    if (_mode == DisplayMode::Rolling) {
        if (_tribe->len() < X_POINTS * _x_rate) {
            len = _tribe->len();
        } else {
            len = (int) (X_POINTS * _x_rate);
            start_pos = _tribe->len() - len;
        }
    } else if (_mode == DisplayMode::Free) {
        if (_tribe->len() < X_POINTS * _x_rate) {
            len = _tribe->len();
        } else {
            len = (int) (X_POINTS * _x_rate);
            start_pos = _h_scroll->value();
        }
    }
    for (int i = 0; i < _tribe->size(); ++i) {
        const Tribe::Cell &iter = (*_tribe)[i];
        const Combine::Cell &com = (*_combine)[i];
        if (!(_tribe->styles())[i].display()) {
            continue;
        }
        const Tribe::Style &cfg = _tribe->styles()[i];
        QColor color = cfg.color();
        glColor3d(color.redF(), color.greenF(), color.blueF());
//        if (i == _current_index) {
//            glLineWidth(cfg.width() + 1);
//        } else {
//            glLineWidth(cfg.width());
//        }
        glLineWidth(cfg.width());
#if 0
        glBegin(GL_LINES);
        float y_cal = 0;src/gui/Sketch.cpp:181
            if (j) {
                glVertex2f(j, y_cal);
            } else {
                glVertex2f(j, genY(iter[j + start_pos], cfg));
            }src/gui/Sketch.cpp:180
            y_cal = genY(iter[j src/gui/Sketch.cpp:181+ start_pos + 1], cfg);
            glVertex2f(j + 1, y_cal);
        }
        glEnd();
#endif
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < len; ++j) {
            glVertex2f(j, com.data()[(j + start_pos) * 2 + 1]);
        }
        glEnd();
    }
}
*/

void Sketch::plotCurves()
{
    if (_smooth) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    } else {
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
    }
    switch (_mode) {
        case Rolling:
            break;
        case Free:
#ifdef VERTEX
            {
            for (int i = 0; i < _tribe->size(); ++i) {
                if (!_tribe->style(i).display()) {
                    continue;
                }
                QColor color = _tribe->style(i).color();
                qDebug() << "Sketch::plotCurves() color: " << color;
                glColor3d(color.redF(), color.greenF(), color.redF());
                glLineWidth(_tribe->style(i).width());
                glBindVertexArray(_vaos[i]);
                glDrawArrays(GL_LINE_STRIP, 0, _points);
                glFlush();
            }
            qDebug() << "Sketch::plotCurves() points: " << _points;
            glBegin(GL_LINES);
            glVertex2d(0, 0);
            glVertex2d(5, 1000);
            glEnd();
            glFlush();
            break;
        }
#else
        {

            for (int i = 0; i < _tribe->size(); ++i) {
                if (!_tribe->style(i).display()) {
                    continue;
                }
                QColor color = _tribe->style(i).color();
//                qDebug() << "Sketch::plotCurves() color: " << color;
                glColor3d(color.redF(), color.greenF(), color.blueF());
                glLineWidth(_tribe->style(i).width());
                glBegin(GL_LINE_STRIP);
                for (int j = 0; j < _points - 1; ++j) {
                    glVertex2f((*_combine)[i].data()[j * 2],
                               (*_combine)[i].data()[j * 2 + 1]);
                    glVertex2f((*_combine)[i].data()[(j + 1) * 2],
                               (*_combine)[i].data()[(j + 1) * 2 + 1]);
                }
                glEnd();
                glFlush();
            }
            break;
        }
#endif
        case Waiting:
            break;
        case Empty:
            break;
    }
}


void Sketch::plotXAxis()
{

}

void Sketch::plotYGrid()
{

}

void Sketch::plotVernier()
{
    if (!_vernier) {
        return;
    }
    int data_pos = _h_scroll->value() + _vernier_pos;
    const int vernier_dist = 10;
    QPainter painter;
    painter.begin(this);
    QColor color(0xffffff);
    QBrush brush(color);
    QPen pen(color);
    QFont font("Helvetica", 10);
    int row_height = font.pointSize() + 4;
    font.setStyleHint(QFont::Helvetica, QFont::OpenGLCompatible);
    QFontMetrics metrics(font, this);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.setFont(font);
    int x = xGlToQt(_vernier_pos);
    int y1 = yGlToQt(Y_BOTTOM);
    int y2 = yGlToQt(Y_POINTS);
    painter.drawLine(x, y1, x, y2);
    if (_tribe->len() <= _vernier_pos || data_pos >= _tribe->len()) {
        return;
    }
    QString time_value = QString("时间 %1 s").arg(data_pos / 100.0, 0, 'f', 2);
    int max_time_width = metrics.boundingRect(time_value).width();
    if (rect().width() - x < max_time_width + vernier_dist) {
        painter.drawText(QRect(x - vernier_dist - max_time_width,
                               y2 - 10 - row_height,
                               max_time_width, row_height), time_value,
                         QTextOption(Qt::AlignRight | Qt::AlignHCenter));
    } else {
        painter.drawText(x + vernier_dist, y2 - 10, time_value);
    }
    int max_name_width = 0;
    for (int i = 0; i < _tribe->size(); ++i) {
        if (!_tribe->style(i).display()) {
            continue;
        }
        int w = metrics.boundingRect(_tribe->style(i).name()).width();
        if (max_name_width < w) {
            max_name_width = w;
        }
    }
    int max_value_width = 40;
    for (int i = 0; i < _tribe->size(); ++i) {
        if (!_tribe->style(i).display()) {
            continue;
        }
        color = _tribe->style(i).color();
        pen.setColor(color);
        pen.setWidth(_tribe->style(i).width());
        painter.setPen(pen);
        font.setBold(i == _current_index);
        font.setUnderline(i == _current_index);
        painter.setFont(font);
        if (i == _current_index) {
            float y0 = genY((*_tribe)[i][data_pos], _tribe->style(i));
            int d = _tribe->style(i).width() * 2 + 4;
            painter.drawEllipse(x - d / 2, yGlToQt(y0) - d / 2, d, d);
        }
        y2 += row_height;
        QString value = QString("%1").arg((*_tribe)[i][data_pos], 0, 'f', 0);
        QString name = _tribe->style(i).name();
        QString suffix;
        switch ((*_tribe)[i].fillType(data_pos)) {
            case Tribe::FakeByZero:
                suffix = "-";
                break;
            case Tribe::FakeByPrevious:
                suffix = "+";
                break;
            default:
                break;
        }
        if (rect().width() - x > max_name_width + max_value_width + 2 * vernier_dist) {
            painter.drawText(x + vernier_dist, y2, value);
            painter.drawText(x + vernier_dist + max_value_width, y2, name + suffix);
        } else if (rect().width() - x < max_name_width + max_value_width + 2 * vernier_dist &&
                   rect().width() - x > max_value_width + vernier_dist) {
            painter.drawText(x + 10, y2, value);
            painter.drawText(
                    QRect(x - vernier_dist - max_name_width - 10, y2 - row_height,
                          max_name_width + 10, row_height),
                    suffix + name,
                    QTextOption(Qt::AlignRight | Qt::AlignHCenter)
            );
        } else {
            painter.drawText(
                    QRect(x - vernier_dist - max_value_width, y2 - row_height, max_value_width,
                          row_height),
                    value,
                    QTextOption(Qt::AlignRight | Qt::AlignHCenter)
            );
            painter.drawText(
                    QRect(x - 2 * vernier_dist - max_name_width - max_value_width - 10,
                          y2 - row_height, max_name_width + 10, row_height),
                    suffix + name,
                    QTextOption(Qt::AlignRight | Qt::AlignHCenter)
            );
        }
    }
    painter.end();
}

void Sketch::drawGlString(double x0, double y0, const QString &str,
                          const QColor &color, const QFont &font)
{
    int x, y;
    pointGlToQt(x0, y0, x, y);
    QPainter painter;
    painter.begin(this);
    painter.setPen(color);
    painter.setFont(font);
    painter.drawText(x, y + font.pointSize() / 2, str);
    painter.end();
}

void Sketch::drawQtString(int x0, int y0, const QString &str,
                          const QColor &color, const QFont &font)
{

}

void Sketch::pointGlToQt(double x0, double y0, int &x1, int &y1)
{
    x1 = xGlToQt(x0);
    y1 = yGlToQt(y0);
}

void Sketch::pointQtToGl(int x0, int y0, double &x1, double &y1)
{
    x1 = xQtToGl(x0);
    y1 = yQtToGl(y0);
}

void Sketch::wheelEvent(QWheelEvent *event)
{
    //emitMessage(Debug, QString("滚轮改变量 %1").arg(event->delta()));
    scrollMove(event->delta());
}

void Sketch::keyPressEvent(QKeyEvent *event)
{

    int direction = 0;
    if (event->key() == Qt::Key_Right) {
        direction = -1;
    }
    if (event->key() == Qt::Key_Left) {
        direction = 1;
    }
    scrollMove(120 * direction);
    if (event->isAutoRepeat()) {
        return;
    }
    if (event->key() == Qt::Key_L) {
        _vernier ^= 1;
        update();
    }
    if (event->key() == Qt::Key_F12) {
        event->setAccepted(false);
    }
    emitMessage(Debug, QString("按下 %1").arg(event->key()));
}

void Sketch::drawFocusSign()
{
    QPainter painter;
    painter.begin(this);
    painter.setBrush(QColor(0xffffff));
    painter.drawEllipse(5, 5, 10, 10);
    painter.end();
}

void Sketch::scrollMove(int angle)
{
    if (!angle) {
        return;
    }
    auto delta = (int) (_h_scroll->maximum() * (angle / 2000.0));
    int value = _h_scroll->value() - delta;
    if (value > _h_scroll->maximum()) {
        value = _h_scroll->maximum();
    }
    if (value < _h_scroll->minimum()) {
        value = _h_scroll->minimum();
    }
    _h_scroll->setValue(value);
}

void Sketch::keyReleaseEvent(QKeyEvent *event)
{
}

void Sketch::mouseMoveEvent(QMouseEvent *event)
{
//    int x, y;
//    axisGlToQt(_vernier_pos, 0, x, y);
//    if (event->pos().x() - x < 5 && event->pos().x() - x > -5) {
//        setCursor(Qt::SizeHorCursor);
//    } else {
//        setCursor(Qt::ArrowCursor);
//    }
    if (event->buttons() & Qt::LeftButton) {
        if (event->pos().x() <= 0 || event->pos().x() >= rect().width()) {
            event->setAccepted(false);
            return;
        }
        int x0 = event->pos().x();
//        emitMessage(Debug, QString("鼠标位置 %1").arg(x0));
        double x, y;
        pointQtToGl(x0, 0, x, y);
        if (x < 0) {
            x = 0;
        }
        if (x > X_POINTS * _x_rate) {
            x = X_POINTS * _x_rate;
        }
        _vernier_pos = int(x);
        update();
    }
}

int Sketch::xGlToQt(double x)
{
    double left = X_LEFT;
    double w = X_POINTS * _x_rate - X_LEFT;
    return (int) round(rect().width() * (x - left) / w);
}

double Sketch::xQtToGl(int x)
{
    double w = X_POINTS * _x_rate - X_LEFT;
    return X_LEFT + (double) x / rect().width() * w;
}

int Sketch::yGlToQt(double y)
{
    double top = Y_POINTS;
    double h = Y_POINTS - Y_BOTTOM;
    return (int) round(rect().height() * (top - y) / h);
}

double Sketch::yQtToGl(int y)
{
    double h = Y_POINTS - Y_BOTTOM;
    return Y_BOTTOM + (double) (rect().height() - y) / rect().height() * h;
}

void Sketch::mousePressEvent(QMouseEvent *event)
{
//    if (event->button() == Qt::RightButton) {
//        _right_mouse_press_pos = event->x();
//    }
}

void Sketch::mouseReleaseEvent(QMouseEvent *event)
{
//    if (event->button() == Qt::RightButton) {
//        _right_mouse_release_pos = event->x();
//        int start_pos = static_cast<int>((__min(_right_mouse_press_pos , _right_mouse_release_pos)  -
//                _left_axis_width) * (X_RIGHT));
//        _x_rate = (double) rect().width() / (abs(_right_mouse_press_pos -
//                                                 _right_mouse_release_pos));
//
//    }
}



