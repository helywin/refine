//
// Created by jiang.wenqiang on 2018/7/26.
//

#ifndef REFINE_SKETCH_H
#define REFINE_SKETCH_H

#include <QtWidgets/QOpenGLWidget>
#include "Painter.h"

class Sketch : public QOpenGLWidget {
Q_OBJECT
public:
    explicit Sketch(QWidget *parent = nullptr);

    ~Sketch() final = default;

    inline void set_param(int freq, int num, int ps, int width) {
        paint.setParams(freq, num, ps, width);
    }

private:
    Painter paint;
protected:
    void paintEvent(QPaintEvent *e) override;

public slots:

    void animate();
};


#endif //REFINE_SKETCH_H
