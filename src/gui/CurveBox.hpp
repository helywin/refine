//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_CURVEBOX_HPP
#define REFINE_CURVEBOX_HPP

#include <QtWidgets/QDockWidget>

class CurveBox : public QDockWidget
{
Q_OBJECT
private:
public:
    explicit CurveBox(QWidget *parent);
private:
    void setup();
};


#endif //REFINE_CURVEBOX_HPP
