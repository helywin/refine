//
// Created by jiang.wenqiang on 2018/10/23.
//

#include <QtWidgets/QAction>
#include <QtGui/QResizeEvent>
#include <QtCore/QDebug>
#include "CurveBox.hpp"
#include "CurvePanel.hpp"

CurveBox::CurveBox(QWidget *parent) :
    QDockWidget(parent)
{
    setup();
}

void CurveBox::setup()
{
    setWindowTitle(tr("曲线"));
    _curve_panel = new CurvePanel(this);
    setWidget(_curve_panel);
}

void CurveBox::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    qDebug() << event->size();
    qDebug() << _curve_panel->width();
}

void CurveBox::closeEvent(QCloseEvent *event)
{
    _visible->setChecked(false);
}
