//
// Created by jiang.wenqiang on 2018/10/31.
//

#ifndef REFINE_CURVEPANEL_HPP
#define REFINE_CURVEPANEL_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

class Curve;

class CurvePanel : public QWidget
{
Q_OBJECT
public:
    const int TABLE_ROW_HEIGHT = 20;
private:
    QVBoxLayout *_layout;
    QTableWidget *_list;
    Curve *_curve;

public:
    explicit CurvePanel(QWidget *parent);

    inline void setCurve(Curve *curve)
    {
        _curve = curve;
        updateCurve();
    }

    void updateCurve();

private:

    void setup();
};


#endif //REFINE_CURVEPANEL_HPP
