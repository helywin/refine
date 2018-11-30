//
// Created by jiang.wenqiang on 2018/11/23.
//

#ifndef REFINE_MYOPENGLWIDGET_HPP
#define REFINE_MYOPENGLWIDGET_HPP

#include <QtWidgets/QOpenGLWidget>
#include <QtGui/QOpenGLExtraFunctions>

class MyOpenGLWidget :
        public QOpenGLWidget,
        protected QOpenGLExtraFunctions
{
Q_OBJECT
public:
    MyOpenGLWidget();
private:
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};


#endif //REFINE_MYOPENGLWIDGET_HPP
