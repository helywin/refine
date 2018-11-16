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
#include "CurveFilter.hpp"

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
    _content = new QWidget(this);
    setWidget(_content);
    _layout = new QVBoxLayout(_content);
    _layout->setMargin(5);
    _layout->setContentsMargins(0, 10, 10, 0);
    _filter = new CurveFilter(_model, this);
    _content->setLayout(_layout);
    _layout->addWidget(_filter);
    _model = new TribeModel(_content);
    _proxy = new TribeSortModel(_content);
    _proxy->setSourceModel(_model);
    _selection = new QItemSelectionModel(_model);
    _h_header = new QHeaderView(Qt::Horizontal);
    _v_header = new QHeaderView(Qt::Vertical);
    _view = new TribeView(_proxy, _selection, _h_header, _v_header, _content);
//    _view->setSortingEnabled(true);
    _h_header->setParent(_view);
    _layout->addWidget(_view);
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
    int index = _view->currentIndex().row();
    emitMessage(Debug, tr("当前行 %1").arg(index));
    if (index < 0) {
        _sketch->setCurrentIndex(index, false);
    } else {
        _sketch->setCurrentIndex(index, _tribe->style(index).display());
    }
    _sketch->update();
}
