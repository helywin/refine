//
// Created by jiang.wenqiang on 2018/9/28.
//

#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <QtCore/QVector>
#include "Sketch.hpp"
#include "Revolve.hpp"

const double Sketch::X_LEFT = 0.0;
const int Sketch::X_POINTS = 2000;
const double Sketch::Y_BOTTOM = 0.0;
const double Sketch::Y_POINTS = 4096.0;

Sketch::Sketch(QWidget *parent, Revolve *revolve, Message *message) :
        QOpenGLWidget(parent),
        Message(message),
        _tribe(&revolve->tribe()),
        _combine(&revolve->combine()),
        _mode(Empty),
        _x_rate(1),
        _y_rate(1),
        _x_start(0),
        _current_index(-1),
        _smooth(true),
        _vernier_visible(true),
        _vernier_fix(false)
{
    setMinimumWidth(200);
    setFocusPolicy(Qt::ClickFocus);
    _verniers.append(Vernier({0, 0, 0}));
//    setMouseTracking(true);
}

void Sketch::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
//    glEnable(GL_DEPTH_TEST);
//    setAutoBufferSwap(true);
    setSmooth(_smooth);
}

void Sketch::resizeGL(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //让顶上和底部多出一像素，关闭反走样值为0的曲线不会消失
    glOrtho(0, X_POINTS, _y_start - Y_POINTS / (double) w,
            _y_start + Y_POINTS * (1 + 1 / (double) w), 0, 100);

}

void Sketch::paintGL()
{
    calculateXEnd();
    _x_sec = 1 / _x_rate;
    glClear(GL_COLOR_BUFFER_BIT);
//    plotXAxis();
    plotYGrid();
    plotXGrid();
    plotCurves();
    plotVerniers();
//    drawFocusSign();
    glFlush();
}

void Sketch::setSmooth(bool enable)
{
    _smooth = enable;
    QOpenGLWidget::repaint();
}

void Sketch::init()
{
#ifdef VERTEX
    _points = _tribe->len();
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
                for (int j = 0; j < xPoints(); ++j) {
                    glVertex2f(float(j * _x_sec), (*_combine)[i].data()[j + _x_start]);
                }
                if (_x_start + xPoints() < _tribe->len()) {
                    glVertex2f(float(xPoints() * _x_sec),
                               (*_combine)[i].data()[xPoints() + _x_start]);
                }
                glEnd();
                glFlush();
                glPointSize(_tribe->style(i).width() * 2 + 2);
                glBegin(GL_POINTS);
                if ((double) rect().width() / xPoints() >= 15) {
                    for (int j = 0; j < xPoints(); ++j) {
                        if (_tribe->at(i).fillType(j + _x_start) == Tribe::Fill::Data) {
                            glVertex2f(float(j * _x_sec), (*_combine)[i].data()[j + _x_start]);
                        }
                    }
                    if (_x_start + xPoints() < _tribe->len()) {
                        glVertex2f(float(xPoints() * _x_sec),
                                   (*_combine)[i].data()[xPoints() + _x_start]);
                    }
                }
                glEnd();
                glFlush();
            }
            break;
        }
#endif
        case Waiting: {
            QPainter painter;
            painter.begin(this);
            int size = 80;
            QFont font("微软雅黑", size);
            painter.setFont(font);
            painter.setPen(Qt::white);
            painter.drawText(rect().width() / 2 - size, rect().height() / 2 + size / 2, tr("式"));
            painter.end();
            break;
        }
        case Empty: {
            QPainter painter;
            painter.begin(this);
            int size = 80;
            QFont font("微软雅黑", size);
            painter.setFont(font);
            painter.setPen(Qt::white);
            painter.drawText(rect().width() / 2 - size, rect().height() / 2 + size / 2, tr("空"));
            painter.end();
            break;
        }
    }
}

#define X_MINIMUM_PIXEL 60
#define X_MAXIMUM_PIXEL 70

void Sketch::plotXGrid()
{
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    if (_current_index < 0) {
        return;
    }
    int num = 10;
    int range = rect().width();
    while (range / num > X_MAXIMUM_PIXEL) {
        num += 1;
    }
    while (range / num < X_MINIMUM_PIXEL && num > 1) {
        num -= 1;
    }
    if (num != _x_graduate_num) {
        _x_graduate_num = num;
    }
    QColor color(0, 96, 48);
    glLineStipple(4, 0x5555);
    glLineWidth(1);
    glEnable(GL_LINE_STIPPLE);
    glColor3d(color.redF(), color.greenF(), color.blueF());
    glBegin(GL_LINES);
    for (int i = 0; i <= _x_graduate_num; ++i) {
        double x = (X_POINTS) * (i / double(_x_graduate_num));
        glVertex2d(x, 0);
        glVertex2d(x, Y_POINTS);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glFlush();
}

#undef X_MINIMUM_PIXEL
#undef X_MAXIMUM_PIXEL


#define Y_MINIMUM_PIXEL 40
#define Y_MAXIMUM_PIXEL 50

void Sketch::plotYGrid()
{
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
    if (_current_index < 0) {
        return;
    }
//    qDebug() << "Sketch::plotYGrid()";
    Tribe::Style &style = _tribe->style(_current_index);
    int num = 10;
    int range = rect().height() - 2;
    bool is_logic = style.rangeOut()[1] - style.rangeOut()[0] < num;
//    qDebug() << "SketchY::plotYAxis() is_logic: " << is_logic;
    if (is_logic) {
        num = style.rangeOut()[1] - style.rangeOut()[0];
    }
    while (range / num > Y_MAXIMUM_PIXEL && !is_logic) {
        num += 1;
    }
    while (range / num < Y_MINIMUM_PIXEL && num > 1) {
        num -= 1;
    }
    if (num != _y_graduate_num) {
        _y_graduate_num = num;
    }

    QColor color(0, 96, 48);
    glLineStipple(4, 0x5555);
    glLineWidth(1);
    glEnable(GL_LINE_STIPPLE);
    glColor3d(color.redF(), color.greenF(), color.blueF());
    glBegin(GL_LINES);
    for (int i = 0; i <= _y_graduate_num; ++i) {
        double y = (Y_POINTS) * (i / double(_y_graduate_num));
        glVertex2d(0, y);
        glVertex2d(X_POINTS, y);
    }
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glFlush();
}

#undef Y_MINIMUM_PIXEL
#undef Y_MAXIMUM_PIXEL

void Sketch::plotVerniers()
{
    if (!_vernier_visible) {
        return;
    }
//    int data_pos = _h_scroll->value() + _vernier_pos;
    QPainter painter;
    painter.begin(this);
    for (const auto &v : _verniers) {
        int data_pos;
        if (_vernier_fix) {
            data_pos = qRound(v.pos * _x_rate) + v.start;
        } else {
            data_pos = qRound(v.pos * _x_rate) + _x_start;
        }
        const int vernier_dist = 10;
        const int font_size = 10;
        QColor color(0xffffff);
        QBrush brush(color);
        QPen pen(color);
        QFont font("Helvetica", font_size);
        int row_height = font.pointSize() + 4;
        font.setStyleHint(QFont::Helvetica, QFont::OpenGLCompatible);
        QFontMetrics metrics(font, this);
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.setFont(font);
        int x;
        if (_vernier_fix) {
            x = xGlToQt(v.start + v.pos - _x_start);
        } else {
            x = xGlToQt(v.pos);
        }
        int y1 = yGlToQt(Y_BOTTOM);
        int y2 = yGlToQt(Y_POINTS);
        painter.drawLine(x, y1, x, y2);
        if (_tribe->len() <= v.pos || data_pos >= _tribe->len()) {
            return;
        }
        QString time_value = QString("时间 %1 s").arg(data_pos / 100.0, 0, 'f', 2);
        int max_time_width = metrics.boundingRect(time_value).width();
        if (rect().width() - x < max_time_width + vernier_dist) {
            painter.drawText(QRect(x - vernier_dist - max_time_width,
                                   y2 - font_size - row_height,
                                   max_time_width, row_height), time_value,
                             QTextOption(Qt::AlignRight | Qt::AlignHCenter));
        } else {
            painter.drawText(x + vernier_dist, y2 - font_size, time_value);
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
        int max_cnt = rect().height() / (font_size + vernier_dist);
        int cnt = 0;
        for (int i = 0; i < _tribe->size(); ++i) {
            if (!_tribe->style(i).display()) {
                continue;
            }
            if (cnt > max_cnt) {
                break;  //多画也是看不到，反而增加重绘时间
            }
            color = _tribe->style(i).color();
            pen.setColor(color);
            pen.setWidth(_tribe->style(i).width());
            painter.setPen(pen);
            font.setBold(i == _current_index);
            font.setUnderline(i == _current_index);
            painter.setFont(font);
            if (i == _current_index) {
//            float y0 = genY((*_tribe)[i][data_pos], _tribe->style(i));
                float y0 = (*_combine)[i].data()[data_pos];
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
                              max_name_width + font_size, row_height),
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
    }
    painter.end();
}

void Sketch::plotPatterns()
{

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
        return;     //后面的事件不会是重复按键的
    }
    if (event->key() == Qt::Key_L) {
        _vernier_visible ^= 1;
        update();
    }
    if (event->key() == Qt::Key_F12) {
        event->setAccepted(false);
    }
    if (event->key() == Qt::Key_Equal) {
        emit zoomXPlus();
    }
    if (event->key() == Qt::Key_Minus) {
        emit zoomXMinus();
    }
    if (event->key() == Qt::Key_D) {
        emit zoomXDefault();
    }
    if (event->key() == Qt::Key_M) {
        emit zoomXMinimum();
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
        if (x > X_POINTS) {
            x = X_POINTS;
        }
        _verniers[0].pos = x;
        _verniers[0].start = _x_start;
        update();
    }
}

int Sketch::xGlToQt(double x)
{
    double left = X_LEFT;
    double w = X_POINTS - X_LEFT;
    return qRound(rect().width() * (x - left) / w);
}

double Sketch::xQtToGl(int x)
{
    double w = X_POINTS - X_LEFT;
    return X_LEFT + (double) x / rect().width() * w;
}

int Sketch::yGlToQt(double y)
{
    double top = Y_POINTS;
    double h = Y_POINTS - Y_BOTTOM;
    return qRound(rect().height() * (top - y) / h);
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



