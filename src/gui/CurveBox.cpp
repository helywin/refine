//
// Created by jiang.wenqiang on 2018/10/23.
//

#include <QtWidgets/QAction>
#include <QtGui/QResizeEvent>
#include <QtCore/QDebug>
#include "CurveBox.hpp"
#include "CurvePanel.hpp"
#include "TribeView.hpp"
#include "Sketch.hpp"

CurveBox::CurveBox(Tribe *tribe, QWidget *parent) :
    _tribe(tribe),
    QDockWidget(parent)
{
    setup();
}

void CurveBox::setup()
{
    setWindowTitle(tr("曲线"));
//    _curve_panel = new CurvePanel(this);
    _model = new TribeModel(this);
    _selection = new QItemSelectionModel(_model);
    _h_header = new QHeaderView(Qt::Horizontal);
    _v_header = new QHeaderView(Qt::Vertical);
    _view = new TribeView(_model, _selection, _h_header, _v_header, this);
    _h_header->setParent(_view);
    setWidget(_view);
}

void CurveBox::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    qDebug() << event->size();
//    qDebug() << _curve_panel->width();
}

void CurveBox::connectModelToSketch(Sketch *sketch)
{
    connect(_model, &TribeModel::tribeChanged,
            sketch, &Sketch::updateGL, Qt::DirectConnection);
}
