//
// Created by jiang.wenqiang on 2018/9/28.
//

#include <QtCore/QtMath>
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
        _mode(Empty),
        _x_start(0),
        _x_rate(1),
        _y_start(0.0),
        _y_rate(1.0),
        _background_color(0, 0, 0),
        _grid_color(0, 96, 48),
        _current_index(-1),
        _smooth(true),
        _vernier_visible(true),
        _vernier_fix(false),
        _vernier_back(true),
        _movement(MoveNone),
        _zoom_x(true),
        _zoom_y(false),
        _plot_zoom_rect(false),
        _zoom_finish(true),
        _modifiers(Qt::NoModifier),
        _fps_counter(-1),
        _fps(60)
{
    setMinimumWidth(200);
    setFocusPolicy(Qt::ClickFocus);
    _verniers.append(Vernier({0, 0, 0}));
    setMouseTracking(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
}

void Sketch::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor((float) _background_color.redF(), (float) _background_color.greenF(),
                 (float) _background_color.blueF(), (float) _background_color.alphaF());
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
    currentIndexOverflow();
    plotYGrid();
    plotXGrid();
    plotCurves();
    plotMarks();
    plotFlags();
    plotVerniers();
    plotZoomRect();
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

/*!
 * @brief 绘制曲线
 */

void Sketch::plotCurves()
{
    if (_smooth) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    } else {
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_BLEND);
    }
    switch (_mode) {
        case Rolling:
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
                const Tribe::Style &st = _tribe->style(i);
                if (!st.display()) {
                    continue;
                }
                QColor color = st.color();
//                qDebug() << "Sketch::plotCurves() color: " << color;
                glColor3d(color.redF(), color.greenF(), color.blueF());
                glLineWidth(st.width());
                if (needToCompress()) { //对于大量曲线采取抽样绘制会提升效率同时减少内存
                    double end;
                    if (_tribe->len() < xPoints()) {
                        end = _tribe->len() / _x_rate;
                    } else {
                        end = X_POINTS;
                    }
                    glBegin(GL_LINE_STRIP);
                    for (int j = 0; j < end; ++j) {
                        int index = qFloor(j * _x_rate) + _x_start;
                        /////////////////////////////////////////////////////////////////////////
                        if (index > _tribe->len() || index < 0) {
                            qDebug() << "line:256 tribe.len: " << _tribe->len() << "_x_start: "
                                     << _x_start << "end: " << end << "j: " << j;
                        }
                        Q_ASSERT(index < _tribe->len() && index >= 0);
                        glVertex2f(float(j * _x_sec * _x_rate),
                                   yToGl((*_tribe)[i].data(index), st));
                    }
                    glEnd();
                    glFlush();
                    glPointSize(_tribe->style(i).width() * 2 + 2);
                    glBegin(GL_POINTS);
                    if ((double) rect().width() / xPoints() >= 15) {
                        for (int j = 0; j < end; ++j) {
                            int index = qRound(j * _x_rate) + _x_start;
                            Q_ASSERT(index < _tribe->len() && index >= 0);
                            if (_tribe->at(i).fillType(index) == Tribe::Fill::Data) {
                                glVertex2f(float(j * _x_sec),
                                           yToGl((*_tribe)[i].data(index), st));
                            }
                        }
                    }
                    glEnd();
                    glFlush();
                } else {
                    int end;
                    if (_tribe->len() < xPoints()) {
                        end = _tribe->len();
                    } else {
                        end = xPoints();
                    }
                    glBegin(GL_LINE_STRIP);
                    //绘制曲线
                    for (int j = 0; j < end; ++j) {
                        /////////////////////////////////////////////////////////////
                        if (j + _x_start > _tribe->len() || j + _x_start < 0) {
                            qDebug() << "line:292 tribe.len: " << _tribe->len() << "_x_start: "
                                     << _x_start << "j: " << j;
                        }
                        Q_ASSERT(j + _x_start < _tribe->len() && j + _x_start >= 0);
                        glVertex2f(float(j * _x_sec),
                                   yToGl((*_tribe)[i].data(j + _x_start), st));
                    }
                    int point_last = _tribe->len() - _x_start - xPoints();
                    for (int k = 0; k < point_last; ++k) {
                        glVertex2f(float((xPoints() + k) * _x_sec),
                                   yToGl((*_tribe)[i].data(_x_start + xPoints() + k), st));
                    }

                    glEnd();
                    glFlush();
                    //绘制点
                    glPointSize(_tribe->style(i).width() * 2 + 2);
                    glBegin(GL_POINTS);
                    if ((double) rect().width() / xPoints() >= 15) {
                        for (int j = 0; j < end; ++j) {
                            Q_ASSERT(j + _x_start < _tribe->len() && j + _x_start >= 0);
                            if (_tribe->at(i).fillType(j + _x_start) == Tribe::Fill::Data) {
                                glVertex2f(float(j * _x_sec),
                                           yToGl((*_tribe)[i].data(j + _x_start), st));
                            }
                        }
                        for (int k = 0; k < point_last; ++k) {
                            glVertex2f(float((xPoints() + k) * _x_sec),
                                       yToGl((*_tribe)[i].data(_x_start + xPoints() + k), st));
                        }
                    }
                    glEnd();
                    glFlush();
                }
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
        case Frames: {
            QPainter painter;
            painter.begin(this);
            int size = 40;
            QFont font("微软雅黑", size);
            painter.setFont(font);
            painter.setPen(Qt::white);
            painter.drawText(rect().width() / 2 - size * 7, rect().height() / 2 + size / 2,
                             tr("已采集%1s报文数据")
                                     .arg(_tribe->len() * _tribe->msec() / 1000.0,
                                          0, 'f', 2));
            painter.end();
            break;
        }
    }
}

/*!
 * @brief 绘制标记虚线
 */

void Sketch::plotMarks()
{
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);

//    glLineStipple(6, 0x5555);
//    glLineStipple(1, 0x0fff);
    glLineWidth(1);
    glEnable(GL_LINE_STIPPLE);
    for (const auto mark : _tribe->marks()) {
        switch (mark.type) {
            case Re::DotLine:
                glColor3f(1, 0, 0);
                glLineStipple(2, 0x5555);
                break;
            case Re::DashDotLine:
                glColor3f(0, 1, 0);
                glLineStipple(2, 0xfe38);
                break;
            case Re::DashLine:
                glColor3f(0, 0, 1);
                glLineStipple(1, 0x00ff);
                break;
            case Re::SolidLine:
            default:
                glColor3f(1, 1, 0);
                glLineStipple(1, 0xffff);
                break;
        }
        glBegin(GL_LINES);
        if (mark.pos >= _x_start && mark.pos < _x_start + xPoints()) {
            double x = (mark.pos - _x_start) * _x_sec;
            glVertex2d(x, 0);
            glVertex2d(x, Y_POINTS);
        }
        glEnd();
    }
    glDisable(GL_LINE_STRIP);
    glFlush();
}

#define X_MINIMUM_PIXEL 60
#define X_MAXIMUM_PIXEL 70

/*!
 * @brief 绘制横轴网格
 */

void Sketch::plotXGrid()
{
    if (_tribe->size() == 0) {
        return;
    }
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
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
    glLineStipple(4, 0x5555);
    glLineWidth(1);
    glEnable(GL_LINE_STIPPLE);
    glColor3d(_grid_color.redF(), _grid_color.greenF(), _grid_color.blueF());
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

/*!
 * @brief 绘制纵轴网格
 */

void Sketch::plotYGrid()
{
    if (_current_index < 0 || !_tribe->style(_current_index).display()) {
        return;
    }
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_BLEND);
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

    glLineStipple(4, 0x5555);
    glLineWidth(1);
    glEnable(GL_LINE_STIPPLE);
    glColor3d(_grid_color.redF(), _grid_color.greenF(), _grid_color.blueF());
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

/*!
 * @brief 绘制游标
 */

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
        const int font_size = 13;
        const int vernier_dist = 10;
        QColor color(0xffffff);
        QBrush brush(color);
        QPen pen(color);
        QFont font;
        font.setStyleHint(QFont::Helvetica, QFont::PreferQuality);
        font.setFamily("Helvetica");
        font.setPointSize(font_size);
        font.setBold(true);
//        if (font_size <= 11) {
//        } else {
//            font.setFamily("微软雅黑");
//        }
        QFontMetrics metrics(font, this);
        int row_height = metrics.height();
        painter.setPen(pen);
        painter.setBrush(brush);
        painter.setFont(font);
        int x;
        if (_vernier_fix) {
            x = xGlToQt(v.start + v.pos - _x_start);
        } else {
            x = xGlToQt(v.pos);
        }
        if (x < 1) {
            x = 1;
        }
        int y1 = yGlToQt(Y_BOTTOM);
        int y2 = yGlToQt(Y_POINTS);
        //画竖线
        painter.drawLine(x, y1, x, y2);
        if ((_vernier_fix && data_pos >= _tribe->len())
            || (!_vernier_fix && _tribe->len() <= v.pos)) {     //防止游标越界
            return;
        }
        int max_name_width = 0;
        QVector<int> name_width;
        for (int i = 0; i < _tribe->size(); ++i) {
            name_width.append(metrics.boundingRect(_tribe->style(i).name()).width() + font_size);
            if (_tribe->style(i).display() && max_name_width < name_width.last()) {
                max_name_width = name_width.last();
            }
        }
        int min_value_width = font_size * 4;
        int max_cnt = rect().height() / (metrics.height());
        int cnt = 0;
        QColor background = _background_color;
        background.setAlpha(150);
        for (int i = 0; i < _tribe->size(); ++i) {
//            qDebug() << "Sketch::plotVerniers()";
            if (!_tribe->style(i).display()) {
                continue;
            }
            cnt += 1;
            if (cnt == max_cnt) {
                if (i < _current_index) {
                    i = _current_index;     //保证最后一个能把选中的曲线数据绘制出来
                }
            } else if (cnt > max_cnt) {
                break;  //多画也是看不到，反而增加重绘时间
            }
            const Tribe::Style &st = _tribe->style(i);
            const Tribe::Cell &tr = (*_tribe).cells()[i];
            color = st.color();
            pen.setColor(color);
            pen.setWidth(st.width());
            painter.setPen(pen);
//            font.setBold(i == _current_index);
            font.setUnderline(i == _current_index);
            painter.setFont(font);
            if (i == _current_index) {  //画吸附在曲线上的点还是三角形
                int d = st.width() * 2 + 4;
                Q_ASSERT(data_pos < _tribe->len() && data_pos >= 0);
                int y = yGlToQt(yToGl(tr.data(data_pos), st));
                if (y < 0) {
                    pen.setWidth(0);
                    painter.setPen(pen);
                    QPolygon polygon;
                    polygon.append(QPoint(x, 0));
                    polygon.append(QPoint(x + 4, 8));
                    polygon.append(QPoint(x, 6));
                    polygon.append(QPoint(x - 4, 8));
                    painter.drawPolygon(polygon);
                } else if (y > rect().height()) {
                    pen.setWidth(1);
                    painter.setPen(pen);
                    QPolygon polygon;
                    polygon.append(QPoint(x, rect().height()));
                    polygon.append(QPoint(x + 4, rect().height() - 8));
                    polygon.append(QPoint(x, rect().height() - 6));
                    polygon.append(QPoint(x - 4, rect().height() - 8));
                    painter.drawPolygon(polygon);
                } else {
                    painter.drawEllipse(x - d / 2,
                                        y - d / 2, d, d);
                }
            }
            y2 += row_height;
            Q_ASSERT(data_pos < _tribe->len() && data_pos >= 0);
            QString value = QString("%1").arg(tr.data(data_pos), 0, 'f',
                                              _tribe->style(i).precision());
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
            int max_value_width = min_value_width + _tribe->style(i).precision() * font_size;
            //判断文字的长度来调整显示的文字位置
            if (rect().width() - x > max_name_width + max_value_width + vernier_dist) {
                QRect rect_value(x + vernier_dist, y2 - row_height,
                                 max_value_width, row_height);
                QRect rect_name(x + vernier_dist + max_value_width, y2 - row_height,
                                name_width[i], row_height);
//                painter.drawText(x + vernier_dist, y2, value);
                if (_vernier_back) {
                    painter.fillRect(rect_value, background);
                    painter.fillRect(rect_name, background);
                }
                painter.drawText(rect_value, value,
                                 QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
                painter.drawText(rect_name, name + suffix,
                                 QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
            } else if (rect().width() - x < max_name_width + max_value_width + vernier_dist &&
                       rect().width() - x > max_value_width + vernier_dist) {
                QRect rect_value(x + vernier_dist, y2 - row_height,
                                 max_value_width, row_height);
                QRect rect_name(x - vernier_dist - name_width[i], y2 - row_height,
                                name_width[i], row_height);
                if (_vernier_back) {
                    painter.fillRect(rect_value, background);
                    painter.fillRect(rect_name, background);
                }
                painter.drawText(rect_value, value,
                                 QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
                painter.drawText(rect_name, suffix + name,
                                 QTextOption(Qt::AlignRight | Qt::AlignVCenter));
            } else {
                QRect rect_value(x - vernier_dist - max_value_width, y2 - row_height,
                                 max_value_width, row_height);
                QRect rect_name(x - vernier_dist - name_width[i] - max_value_width,
                                y2 - row_height, name_width[i], row_height);
                if (_vernier_back) {
                    painter.fillRect(rect_value, background);
                    painter.fillRect(rect_name, background);
                }
                painter.drawText(rect_value, value,
                                 QTextOption(Qt::AlignRight | Qt::AlignVCenter));
                painter.drawText(rect_name, suffix + name,
                                 QTextOption(Qt::AlignRight | Qt::AlignVCenter));
            }
        }
    }
    painter.end();
}

/*!
 * @brief 绘制区间
 */

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

#define WHEEL_X_ZOOM_MINUS_RATE 1.5
#define WHEEL_X_ZOOM_PLUS_RATE (1/WHEEL_X_ZOOM_MINUS_RATE)
#define WHEEL_Y_ZOOM_MINUS_RATE 1.5
#define WHEEL_Y_ZOOM_PLUS_RATE (1/WHEEL_Y_ZOOM_MINUS_RATE)
#define CTRL_X_ZOOM_RATE 0.8
#define ALT_X_ZOOM_RATE 0.8
#define CTRL_Y_ZOOM_RATE 0.8
#define ALT_Y_ZOOM_RATE 0.8

void Sketch::wheelEvent(QWheelEvent *event)
{
    if (_tribe->size() == 0) {  //没数据不能操作
        return;
    }
    double x_scale = event->pos().x() / (double) rect().width();
    double y_scale = (rect().height() - event->pos().y()) / (double) rect().height();
    double wheel_rate = event->delta() / 120.0;
    double x_rate = qPow(WHEEL_X_ZOOM_PLUS_RATE, qAbs(wheel_rate));
    double y_rate = qPow(WHEEL_Y_ZOOM_PLUS_RATE, qAbs(wheel_rate));
    /*
    double x_rate = qAbs(wheel_rate) * (WHEEL_X_ZOOM_MINUS_RATE - 1);
    double y_rate = qAbs(wheel_rate) * (WHEEL_Y_ZOOM_MINUS_RATE - 1);
    if (_modifiers & Qt::ControlModifier) {
        x_rate *= (1 + CTRL_X_ZOOM_RATE);
        y_rate *= (1 + CTRL_Y_ZOOM_RATE);
    } else if (_modifiers & Qt::AltModifier) {
        x_rate *= ALT_X_ZOOM_RATE;
        y_rate *= ALT_Y_ZOOM_RATE;
    }
    x_rate += 1;
    y_rate += 1;
*/
    if (wheel_rate > 0) {
        zoomInFixed(x_rate, x_scale, y_rate, y_scale);
    } else {
        zoomOutFixed(1 / x_rate, x_scale, 1 / y_rate, y_scale);
    }
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
    if (event->key() == Qt::Key_Equal) {
        zoomInByVernier();
    }
    if (event->key() == Qt::Key_Minus) {
        zoomOutByVernier();
    }
    if (event->isAutoRepeat()) {
        return;     //后面的事件不会是重复按键的
    }
    if (event->key() == Qt::Key_L) {
        _vernier_visible ^= 1;
        emit vernierVisibilityChanged(_vernier_visible);
        update();
    }
    if (event->key() == Qt::Key_F12) {      //防止与界面的演示模式键位冲突
        event->setAccepted(false);
    }
    if (event->key() == Qt::Key_D) {
        emit zoomDefault();
    }
    if (event->key() == Qt::Key_M) {
        emit zoomMinimum();
    }
    if (_mode == Rolling) {
        if (event->key() == Qt::Key_Z) {
            _tribe->addMark(Re::SolidLine);
        }
        if (event->key() == Qt::Key_X) {
            _tribe->addMark(Re::DotLine);
        }
        if (event->key() == Qt::Key_C) {
            _tribe->addMark(Re::DashDotLine);
        }
        if (event->key() == Qt::Key_V) {
            _tribe->addMark(Re::DashLine);
        }
    }
    _modifiers = event->modifiers();
    emitMessage(Re::Debug, QString("按下 %1").arg(event->key()));
}

void Sketch::keyReleaseEvent(QKeyEvent *event)
{
    QWidget::keyReleaseEvent(event);
}

void Sketch::drawFocusSign()
{
    QPainter painter;
    painter.begin(this);
    painter.setBrush(QColor(0xffffff));
    painter.drawEllipse(5, 5, 10, 10);
    painter.end();
}

int Sketch::xGlToQt(double x) const
{
    double left = X_LEFT;
    double w = X_POINTS - X_LEFT;
    return qRound(rect().width() * (x - left) / w);
}

double Sketch::xQtToGl(int x) const
{
    double w = X_POINTS - X_LEFT;
    return X_LEFT + (double) x / rect().width() * w;
}

int Sketch::yGlToQt(double y) const
{
    double top = Y_POINTS;
    double h = Y_POINTS - Y_BOTTOM;
    return qRound(rect().height() * (top - y) / h);
}

double Sketch::yQtToGl(int y) const
{
    double h = Y_POINTS - Y_BOTTOM;
    return Y_BOTTOM + (double) (rect().height() - y) / rect().height() * h;
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
        if (_movement == MoveVernier) {
            int x0 = event->pos().x();
//        emitMessage(Debug, QString("鼠标位置 %1").arg(x0));
            double x = xQtToGl(x0);
            if (x < 0) {
                x = 0;
            }
            if (x > X_POINTS) {
                x = X_POINTS;
            }
            int data_pos;
            if (_vernier_fix) {
                data_pos = qRound(_verniers[0].pos * _x_rate) + _verniers[0].start;
            } else {
                data_pos = qRound(_verniers[0].pos * _x_rate) + _x_start;
            }
            emit vernierMove(xGlToQt(x), data_pos / 100.0);
            _verniers[0].pos = x;
            _verniers[0].start = _x_start;
            update();
        } else if (_movement == MoveZoomRect) {
            QPoint point = event->pos();
            point.setX(limitXInRect(point.x()));
            point.setY(limitYInRect(point.y()));
            if (!_zoom_x && !_zoom_y) {
                return;
            }
            if (_zoom_x && !_zoom_y) {
                point.setY(rect().height());
            }
            if (!_zoom_x && _zoom_y) {
                point.setX(rect().width());
            }
            _zoom_rect.setBottomRight(point);
            update();
        }
    } else if (event->buttons() & Qt::MiddleButton) {
        if (_movement == MoveParallel) {
            setCursor(Qt::ClosedHandCursor);
            parallelMoveByCursor(event->pos());
        }
    } else if (event->buttons() & Qt::RightButton) {}
    else {
//        qDebug() << "move";
        if (_vernier_visible && isMouseOnDragItem(event->pos().x())) {
            setCursor(Qt::SizeHorCursor);
        } else {
            setCursor(Qt::ArrowCursor);
        }
    }
}

#define MOUSE_DRAG_DISTANCE 5

void Sketch::mousePressEvent(QMouseEvent *event)
{
    grabMouse();
    if (event->button() == Qt::LeftButton) {
        int vpos = xGlToQt(_verniers[0].pos);
        if (_vernier_visible && qAbs(vpos - event->pos().x()) < MOUSE_DRAG_DISTANCE) {
            setCursor(Qt::SizeHorCursor);
            _movement = MoveVernier;
        } else if (_tribe->size() != 0) {
            if (!_zoom_finish) {
                return;
            }
            QPoint point = event->pos();
            if (!_zoom_x && !_zoom_y) {
                return;
            }
            _plot_zoom_rect = true;
            if (_zoom_x && !_zoom_y) {
                point.setY(0);
            }
            if (!_zoom_x && _zoom_y) {
                point.setX(0);
            }
            _plot_zoom_rect = true;
            _zoom_finish = false;
            _zoom_rect.setTopLeft(point);
            _zoom_rect.setBottomRight(point);
            setCursor(Qt::CrossCursor);
            _movement = MoveZoomRect;
        }
    } else if (event->button() == Qt::MiddleButton) {
        setCursor(Qt::OpenHandCursor);
        _move_parallel_pos = event->pos();
        _movement = MoveParallel;
    } else if (event->button() == Qt::RightButton) {

    } else {}
}

/*!
 * @brief 鼠标松开事件
 * @param event 事件
 */

void Sketch::mouseReleaseEvent(QMouseEvent *event)
{
    /*
    emitMessage(Debug, QString("x:%1, y:%2").arg(event->pos().x()).arg(event->pos().y()));
    emitMessage(Debug, QString("rect::L:%1 R:%2 T:%3 B:%4")
            .arg(_zoom_rect.left())
            .arg(_zoom_rect.right())
            .arg(_zoom_rect.top())
            .arg(_zoom_rect.bottom())
    );
    */
    if (event->button() == Qt::LeftButton) { //松开鼠标左键
        if (_movement == MoveVernier) {}
        else if (_movement == MoveZoomRect) {
            if (!_zoom_finish) {
                zoomInByRect();
            }
            setCursor(Qt::ArrowCursor);
        }
    }
    if (event->button() == Qt::MiddleButton) { //松开鼠标左键
        setCursor(Qt::ArrowCursor);
    }
    if (event->button() == Qt::RightButton) {
        if (_menu) {
            _menu->exec(event->globalPos());
        }
    }
    event->accept();
    releaseMouse();
    _movement = MoveNone;
    _plot_zoom_rect = false;        //消除对话框后不小心拖动会不能清除放大状态的bug
    _zoom_finish = true;            //消除对话框后不小心拖动会不能清除放大状态的bug
}

#define MINIMUM_ZOOM_RECT_WIDTH 20
#define MINIMUM_ZOOM_RECT_HEIGHT 20

/*!
 * @brief 绘制放大矩形
 */

void Sketch::plotZoomRect()
{
    QColor color_pen(0xffffff);
    QColor color_brush(0xff, 0xff, 0xff, 100);
    if (!_plot_zoom_rect) {
        return;
    }
    if (qAbs(_zoom_rect.width()) < MINIMUM_ZOOM_RECT_WIDTH
        || qAbs(_zoom_rect.height()) < MINIMUM_ZOOM_RECT_HEIGHT) {
        color_pen.setRgb(0xffcccc);
        color_brush = QColor(0xff, 0xcc, 0xcc, 100);
    }
    QRect rect_adjust = _zoom_rect;
    if (rect_adjust.bottom() > rect().height() - 2) {
        rect_adjust.setBottom(rect_adjust.bottom() - 2);
    }
    if (rect_adjust.top() > rect().height() - 2) {
        rect_adjust.setTop(rect_adjust.top() - 2);
    }
    if (rect_adjust.right() > rect().width() - 2) {
        rect_adjust.setRight(rect_adjust.right() - 2);
    }
    if (rect_adjust.left() > rect().width() - 2) {
        rect_adjust.setLeft(rect_adjust.left() - 2);
    }
    QPainter painter;
    painter.begin(this);
    QPen pen(color_pen, 2);
    QBrush brush(color_brush);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(rect_adjust);
    painter.end();
}

void Sketch::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

bool Sketch::isMouseOnDragItem(int x)
{
    for (const auto &ver : _verniers) {
        if (qAbs(xGlToQt(ver.pos) - x) < MOUSE_DRAG_DISTANCE) {
            return true;
        }
    }
//    for (const auto &pat : _patterns) {
//    }
    return false;
}

/*!
 * @brief 计算当前游标上的值的比例
 * @return 比例值
 */

double Sketch::yZoomPos() const
{
    if (_current_index == -1 || _current_index >= _tribe->size()) {
        return 0.5;     //中间缩放会比较舒服
    } else {
        int data_pos;
        if (_vernier_fix) {
            data_pos = qRound(_verniers[0].pos * _x_rate) + _verniers[0].start;
        } else {
            data_pos = qRound(_verniers[0].pos * _x_rate) + _x_start;
        }
        if (_tribe->len() <= _verniers[0].pos || data_pos >= _tribe->len()) {
            return 0.5;
        }
        Q_ASSERT(data_pos < _tribe->len() && data_pos >= 0);
        double rate = yToGl((*_tribe)[_current_index].data(data_pos),
                            _tribe->style(_current_index));
        return rate / Y_POINTS;
    }
}

/*!
 * @brief 根据选定矩形框放大
 */

void Sketch::zoomInByRect()
{
    if (_tribe->size() == 0) {
        return;
    }
    if (_tribe->len() < xPointsF()
        && xQtToGl(qMax(_zoom_rect.left(), _zoom_rect.right()))
           > _tribe->len() / _x_rate) {     //采数据时数据量不多放大会崩溃
        emitMessage(Re::Info, "在崩溃的边缘试探");
        return;
    }
    auto x_rate = (double) (qAbs(_zoom_rect.width())) / rect().width();
    auto x_start = (double) qMin(_zoom_rect.left(), _zoom_rect.right()) / rect().width();
    auto y_rate = (double) (qAbs(_zoom_rect.height())) / rect().height();
    auto y_start = (rect().height() - (double) qMax(_zoom_rect.top(), _zoom_rect.bottom()))
                   / rect().height();
    if (qAbs(_zoom_rect.width()) >= MINIMUM_ZOOM_RECT_WIDTH
        && qAbs(_zoom_rect.height()) >= MINIMUM_ZOOM_RECT_HEIGHT) {
        if (xZoomInLimit()) {
            x_rate = 1;
            x_start = 0;
        }
        if (yZoomInLimit()) {
            y_rate = 1;
            y_start = 0;
        }
        emit zoomIn(x_rate, x_start, y_rate, y_start);
    } else {
        emitMessage(Re::Warning, tr("放大选区过小"));
    }
    _zoom_finish = true;
    _plot_zoom_rect = false;
    update();
}

/*!
 * @brief 根据鼠标位置平移
 * @param pos 当前鼠标位置
 */

void Sketch::parallelMoveByCursor(const QPoint &pos)
{
    if (_tribe->len() < xPoints()) {
        return;
    }
    if (qAbs(pos.x() - _move_parallel_pos.x())
        < rect().width() / xPointsF() / 2) {    //防止放很大后移动速度过慢导致不移动线像
        return;
    }
    QPoint point;
    point.setX(limitXInRect(pos.x()));
    point.setY(limitYInRect(pos.y()));
    double delta_x = -(point.x() - _move_parallel_pos.x())
                     / (double) (rect().width());
    double delta_y = (point.y() - _move_parallel_pos.y())
                     / (double) (rect().height());
    _move_parallel_pos = point;
    emit parallelMove(delta_x, delta_y);
}

#undef MINIMUM_ZOOM_RECT_WIDTH
#undef MINIMUM_ZOOM_RECT_HEIGHT

#define FIX_X_ZOOM_MINUS_RATE 1.5
#define FIX_X_ZOOM_PLUS_RATE (1/FIX_X_ZOOM_MINUS_RATE)
#define FIX_Y_ZOOM_MINUS_RATE 1.5
#define FIX_Y_ZOOM_PLUS_RATE (1/FIX_Y_ZOOM_MINUS_RATE)

/*!
 * @brief 根据游标位置放大
 */

void Sketch::zoomInByVernier()
{
    if (_tribe->size() == 0) {
        return;
    }
    zoomInFixed(FIX_X_ZOOM_PLUS_RATE, _verniers[0].pos / X_POINTS,
                FIX_Y_ZOOM_PLUS_RATE, yZoomPos());
}

/*!
 * @brief 根据游标位置缩小
 */

void Sketch::zoomOutByVernier()
{
    if (_tribe->size() == 0) {
        return;
    }
    zoomOutFixed(FIX_X_ZOOM_MINUS_RATE, _verniers[0].pos / X_POINTS,
                 FIX_Y_ZOOM_MINUS_RATE, yZoomPos());
}

/*!
 * @brief 限制x轴的放大
 * @return x轴是否不能再放大
 */

bool Sketch::xZoomInLimit() const
{
    return xPointsF() < 10;
}

/*!
 * @brief 限制y轴的放大
 * @return y轴是否不能再放大
 */

bool Sketch::yZoomInLimit() const
{
    return yPoints() < 10;
}

/*!
 * @brief 固定放大
 * @param x_rate x轴放大倍率
 * @param x_scale x轴定点位置
 * @param y_rate y轴放大倍率
 * @param y_scale y轴定点位置
 */
void Sketch::zoomInFixed(double x_rate, double x_scale, double y_rate, double y_scale)
{
    double x_start = 0;
    if (_zoom_x && !xZoomInLimit()) {
        x_start = x_scale * (1 - x_rate);
    } else {
        x_rate = 1;
    }
    double y_start = 0;
    if (_zoom_y && !yZoomInLimit()) {
        y_start = y_scale * (1 - y_rate);
    } else {
        y_rate = 1;
    }
    if (_tribe->len() < xPoints()) {    //不能用xPointF，不然一开始就会触发
        x_start = 0;
//        x_rate = _tribe->len() / (double) X_POINTS;
    }
    emit zoomIn(x_rate, x_start, y_rate, y_start);
}

/*!
 * @brief 固定缩小
 * @param x_rate x轴放大倍率
 * @param x_scale x轴定点位置
 * @param y_rate y轴放大倍率
 * @param y_scale y轴定点位置
 */

void Sketch::zoomOutFixed(double x_rate, double x_scale, double y_rate, double y_scale)
{
    double x_start = 0;
    ZoomEdges edge = NoEdge;
    if (_zoom_x) {
        if (x_rate * xRate() > maximumXRate()) {
            edge |= EdgeX;
        } else if (x_scale * (x_rate - 1) * xPointsF()
                   > _x_start) {
            edge |= EdgeLeft;
        } else if ((1 - x_scale) * (x_rate - 1) * xPointsF()
                   > _tribe->len() - _x_start - xPointsF()) {
            edge |= EdgeRight;
        } else {
            x_start = x_scale * (1 - x_rate);
        }
    }
    double y_start = 0;
    if (_zoom_y) {
        if (y_rate * yRate() > maximumYRate()) {
            edge |= EdgeY;
        } else if (y_scale * _y_rate * (y_rate - 1) > _y_start) {
            edge |= EdgeBottom;
        } else if ((1 - y_scale) * _y_rate * (y_rate - 1)
                   > 1 - _y_start - _y_rate) {
            edge |= EdgeTop;
        } else {
            y_start = y_scale * (1 - y_rate);
        }
    }
    if (_tribe->len() < xPointsF()) {
        x_start = 0;
    }
    emit zoomOut(x_rate, x_start, y_rate, y_start, edge);
}

/*!
 * @brief 处理数据重新加载后序号溢出的问题
 */

void Sketch::currentIndexOverflow()
{
    if (_current_index >= _tribe->size()) {
        _current_index = -1;
    }
}

/*!
 * @brief 把x坐标限制在矩形内
 * @param x x坐标
 * @return 限制后坐标
 */

int Sketch::limitXInRect(int x) const
{
    if (x < 0) {
        x = 0;
    } else if (x > rect().width()) {
        x = rect().width() - 1;
    }
    return x;
}

/*!
 * @brief 把y坐标限制在矩形内
 * @param y y坐标
 * @return 限制后坐标
 */

int Sketch::limitYInRect(int y) const
{
    if (y < 0) {
        y = 0;
    } else if (y > rect().height()) {
        y = rect().height() - 1;
    }
    return y;
}

#define FPS_TIMES 5

void Sketch::plotFlags()
{
    if (_fps_counter == -1) {
        _last_repaints = QTime::currentTime();
        _fps_counter = 0;
        return;
    }
    _fps_counter += 1;
    _fps_counter %= FPS_TIMES;
    if (_fps_counter == 0) {
        _fps = 1000.0 / (_last_repaints.msecsTo(QTime::currentTime())) * FPS_TIMES;
        _last_repaints = QTime::currentTime();
    }
    QPainter painter;
    painter.begin(this);
    const int font_size = 10;
    QColor color(0xffffff);
//    QBrush brush(color);
    QPen pen(color);
    QFont font;
    font.setStyleHint(QFont::Helvetica, QFont::PreferQuality);
    font.setFamily("Helvetica");
    font.setPointSize(font_size);
    painter.setFont(font);
    painter.setPen(pen);
//    painter.setPen(brush);
    QFontMetrics metrics(font);
    QString flags = QString("fps:%1").arg(_fps, 2, 'f', 1, QChar(' '));
    if (needToCompress()) {
        flags = QString("抽样绘制 ") + flags;
    }
    int str_width = metrics.boundingRect(flags).width() + font_size;
    QRect str_rect(rect().width() - str_width,
                   0, str_width, metrics.height());
    painter.drawText(str_rect, flags, QTextOption(Qt::AlignRight | Qt::AlignVCenter));
    painter.end();
}

bool Sketch::needToCompress() const
{
    return xPointsF() / rect().width() > 6;
}

void Sketch::updateVernier()
{
    int data_pos;
    if (_vernier_fix) {
        data_pos = qRound(_verniers[0].pos * _x_rate) + _verniers[0].start;
    } else {
        data_pos = qRound(_verniers[0].pos * _x_rate) + _x_start;
    }
    emit vernierMove(xGlToQt(_verniers[0].pos), data_pos / 100.0);
    _verniers[0].start = _x_start;
}


#undef FIX_X_ZOOM_PLUS_RATE
#undef FIX_X_ZOOM_MINUS_RATE
#undef FIX_Y_ZOOM_PLUS_RATE
#undef FIX_Y_ZOOM_MINUS_RATE
