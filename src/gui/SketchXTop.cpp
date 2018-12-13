//
// Created by jiang.wenqiang on 2018/11/30.
//

#include "SketchXTop.hpp"
#include "Tribe.hpp"
#include "Revolve.hpp"

SketchXTop::SketchXTop(Message *message, Revolve *revolve, QWidget *parent) :
        QOpenGLWidget(parent),
        Message(message),
        _tribe(&revolve->tribe()),
        _vernier_pos(0),
        _vernier_visible(true),
        _time(0),
        _current_index(-1)
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
    if (_tribe->len() == 0) {
        return;
    }
    currentIndexOverflow();
    _painter.begin(this);
    const int font_size = 10;
    QColor color(0xffffff);
    QPen pen(color);
    QFont font("Helvetica", font_size);
    font.setStyleHint(QFont::Helvetica, QFont::OpenGLCompatible);
    QFontMetrics metrics(font, this);
    _painter.setPen(pen);
    _painter.setFont(font);
    const int dist = 4;
    const int height = 45;
    QString time_value = QString("时间 %1 s").arg(_time, 0, 'f', 2);
    int time_width = metrics.boundingRect(time_value).width();
    int str_height = metrics.height();
    if (rect().width() - _vernier_pos < time_width + dist) {
        _painter.drawText(QRect(_vernier_pos - dist - time_width,
                                height - str_height,
                                time_width, str_height), time_value,
                          QTextOption(Qt::AlignRight | Qt::AlignVCenter));
    } else {
        _painter.drawText(QRect(_vernier_pos + dist,
                                height - str_height,
                                time_width, str_height), time_value,
                          QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
    }
    if (_current_index != -1 && _tribe->style(_current_index).display()) {
        const Tribe::Style &style = _tribe->style(_current_index);
        pen.setColor(style.color());
        font.setBold(true);
        font.setUnderline(true);
        _painter.setPen(pen);
        _painter.setFont(font);
        QRect name_rect = QRect(0, 0, metrics.boundingRect(style.name()).width() + font_size,
                                str_height);
        _painter.drawText(name_rect, style.name(), QTextOption(Qt::AlignLeft | Qt::AlignVCenter));
    }
    _painter.end();
}

void SketchXTop::currentIndexOverflow()
{
    if (_current_index >= _tribe->size()) {
        _current_index = -1;
    }
}
