//
// Created by jiang.wenqiang on 2018/7/26.
//

#ifndef REFINE_SKETCH_H
#define REFINE_SKETCH_H

#include <QtWidgets/QOpenGLWidget>
#include <QtCore/QThread>
#include <QtGui/QOpenGLFunctions_4_0_Core>
#include "Painter.h"

class Sketch
        : public QOpenGLWidget,
          public QOpenGLFunctions_4_0_Core{
Q_OBJECT
private:
    Painter paint;
public:
    explicit Sketch(QWidget *parent = nullptr);

    ~Sketch() final = default;

    inline void set_param(int num, int ps, int width, int msec) {
        paint.setParams(num, ps, width, msec);
    }

protected:
    void paintEvent(QPaintEvent *e) override;

public slots:

    void animate();
};


#endif //REFINE_SKETCH_H
