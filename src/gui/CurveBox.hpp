//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_CURVEBOX_HPP
#define REFINE_CURVEBOX_HPP

#include <QtWidgets/QDockWidget>

class CurvePanel;

class CurveBox : public QDockWidget
{
Q_OBJECT
private:
    CurvePanel *_curve_panel;
    QAction *_visible;
public:
    explicit CurveBox(QWidget *parent);

    inline CurvePanel *curvePanel()
    {
        return _curve_panel;
    }

    inline void setAction(QAction *action) { _visible = action; }

protected:
    void closeEvent(QCloseEvent *event) override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setup();
};


#endif //REFINE_CURVEBOX_HPP
