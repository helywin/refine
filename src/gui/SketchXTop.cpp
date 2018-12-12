//
// Created by jiang.wenqiang on 2018/11/30.
//

#include "SketchXTop.hpp"

SketchXTop::SketchXTop(Message *message, QWidget *parent) :
        QOpenGLWidget(parent),
        Message(message),
        _vernier_pos(0),
        _vernier_visible(true),
        _time(0)
{
    setup();
}

void SketchXTop::setup()
{
    setFixedHeight(45);
}

void SketchXTop::initializeGL()
{
    QOpenGLWidget::initializeGL();
}

void SketchXTop::resizeGL(int w, int h)
{
    QOpenGLWidget::resizeGL(w, h);
}

void SketchXTop::paintGL()
{
    QPainter painter;
    painter.begin(this);
    const int font_size = 10;
    QColor color(0xffffff);
    QPen pen(color);
    QFont font("Helvetica", font_size);
    int row_height = font.pointSize() + 4;
    font.setStyleHint(QFont::Helvetica, QFont::OpenGLCompatible);
    QFontMetrics metrics(font, this);
    painter.setPen(pen);
    painter.setFont(font);
    const int dist = 10;
    const int height = 20;
    QString time_value = QString("时间 %1 s").arg(_time, 0, 'f', 2);
    int str_width = metrics.boundingRect(time_value).width();
    if (rect().width() - _vernier_pos < str_width + dist) {
        painter.drawText(QRect(_vernier_pos - dist - str_width,
                               height - font_size,
                               str_width, row_height), time_value,
                         QTextOption(Qt::AlignRight | Qt::AlignVCenter));
    } else {
        painter.drawText(QRect(_vernier_pos + dist,
                               height - font_size,
                               str_width, row_height), time_value,
                         QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
    }
    painter.end();
}
