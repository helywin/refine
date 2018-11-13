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

CurveBox::CurveBox(Tribe *tribe, Message *message, QWidget *parent) :
        _tribe(tribe),
        Message(message),
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
    connect(_selection, &QItemSelectionModel::selectionChanged,
            this, &CurveBox::selectionChanged);
}

void CurveBox::connectModelToSketch(Sketch *sketch)
{
    _sketch = sketch;
    connect(_model, &TribeModel::tribeChanged,
            sketch, static_cast<void (Sketch::*)(void)>(&Sketch::update),
            Qt::DirectConnection);
}

void CurveBox::selectionChanged(const QItemSelection &selected,
                                const QItemSelection &deselected)
{
    emitMessage(Debug, tr("当前行 %1").arg(_view->currentIndex().row()));
    _sketch->setCurrentIndex(_view->currentIndex().row());
}
