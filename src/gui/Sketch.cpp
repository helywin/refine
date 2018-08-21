//
// Created by jiang.wenqiang on 2018/7/26.
//

#include "Sketch.h"

void Sketch::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
//    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
    paint.paint(&painter, event);
    painter.end();
}

Sketch::Sketch(QWidget *parent) : QOpenGLWidget(parent) {
}

void Sketch::animate() {
    update();
}
