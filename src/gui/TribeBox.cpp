//
// Created by jiang.wenqiang on 2018/10/23.
//

#include <QtWidgets/QAction>
#include <QtGui/QResizeEvent>
#include <QtCore/QDebug>
#include "TribeBox.hpp"
#include "CurvePanel.hpp"
#include "TribeView.hpp"
#include "Sketch.hpp"
#include "SketchY.hpp"
#include "SketchXTop.hpp"
#include "TribeFilter.hpp"

TribeBox::TribeBox(Tribe *tribe, Message *message, QWidget *parent) :
        QDockWidget(parent),
        _tribe(tribe),
        Message(message)
{
    setup();
}

void TribeBox::setup()
{
    setWindowTitle(tr("曲线"));
    _content = new QWidget(this);
    _search = new QWidget(_content);
    setWidget(_content);
    _layout_search = new QHBoxLayout(_search);
    _check = new QCheckBox(_search);
    _check->setCheckState(Qt::CheckState::Unchecked);
    _search->setLayout(_layout_search);
    _filter = new TribeFilter(_model, _search);
    _layout_search->addWidget(_check);
    _layout_search->addWidget(_filter);
    _layout_search->setContentsMargins(5, 0, 0, 0);
    _layout_content = new QVBoxLayout(_content);
    _layout_content->setMargin(5);
    _layout_content->setContentsMargins(0, 10, 10, 0);
    _content->setLayout(_layout_content);
    _layout_content->addWidget(_search);
    _model = new TribeModel(_content);
    _proxy = new TribeSortModel(_content);
    _proxy->setSourceModel(_model);
    _selection = new QItemSelectionModel(_proxy, this);
//    _h_header = new QHeaderView(Qt::Horizontal);
//    _h_header->setModel(_proxy);
//    _v_header = new QHeaderView(Qt::Vertical);
//    _v_header->setModel(_proxy);
    _view = new TribeView(_proxy, _selection, _content);
    _view->setSortingEnabled(true);
    _view->sortByColumn(0, Qt::AscendingOrder);
//    _h_header->setParent(_view);
//    _v_header->setParent(_view);
    _layout_content->addWidget(_view);
    _complete_model = new CompleteModel(_tribe, this);
    _filter->setCompleteModel(_complete_model);
    _width = new SpinInput(5, 1, this);
    _precision = new SpinInput(5, 0, this);
    _unit = new ComboInput(true,
                           QStringList({"NULL", "mA", "A", "mV", "V", "rpm", "mm", "cm", "m",
                                        "m/s", "m/s²", "On/Off", "Status", "kph", "Hz", "KHz",
                                        "bar", "%", "rad/s", "Nm", "N", "℃"}),
                           this);
    _view->setItemDelegateForColumn(TribeModel::UnitColumn, _unit);
    _view->setItemDelegateForColumn(TribeModel::WidthColumn, _width);
    _view->setItemDelegateForColumn(TribeModel::PrecisionColumn, _precision);
    connect(_selection, &QItemSelectionModel::selectionChanged,
            this, &TribeBox::selectionChanged);
    connect(_check, &QCheckBox::stateChanged,
            this, &TribeBox::setDisplayItem, Qt::DirectConnection);
    connect(_filter, &TribeFilter::filterChanged,
            this, &TribeBox::textFilterChanged);
}

void TribeBox::connectModelToSketch(Sketch *sketch)
{
    _sketch = sketch;
    connect(_model, &TribeModel::tribeChanged,
            sketch, static_cast<void (Sketch::*)(void)>(&Sketch::update),
            Qt::DirectConnection);
    connect(_model, &TribeModel::tribeChanged,
            _complete_model, &CompleteModel::genData, Qt::DirectConnection);
}

void TribeBox::connectModelToSketchY(SketchY *sketch_y)
{
    _sketch_y = sketch_y;
    connect(_model, &TribeModel::tribeChanged,
            sketch_y, static_cast<void (SketchY::*)(void)>(&SketchY::update),
            Qt::DirectConnection);
}

void TribeBox::connectModelToSketchXTop(SketchXTop *sketch_x_top)
{
    _sketch_x_top = sketch_x_top;
    connect(_model, &TribeModel::tribeChanged,
            sketch_x_top, static_cast<void (SketchXTop::*)(void)>(&SketchXTop::update),
            Qt::DirectConnection);
}

void TribeBox::selectionChanged(const QItemSelection &selected,
                                const QItemSelection &deselected)
{
    QItemSelection selection = _proxy->mapSelectionToSource(selected);  //很重要，不然就是当前的行
    if (selection.indexes().isEmpty()) {
        return;
    }
    int index = selection.indexes().first().row();
//    emitMessage(Debug, tr("当前行 %1").arg(index));
    _sketch->setCurrentIndex(index);
    _sketch_y->setCurrentIndex(index);
    _sketch_x_top->setCurrentIndex(index);
    _sketch->update();
    _sketch_y->update();
    _sketch_x_top->update();
}

void TribeBox::setDisplayItem(int state)
{
    switch (state) {
        case Qt::Checked:
            _tribe->displayAll();
            _model->genData(_tribe);
            _sketch->update();
            _complete_model->genData();
            break;
        case Qt::Unchecked:
            _tribe->displayNone();
            _model->genData(_tribe);
            _sketch->update();
            _complete_model->genData();
            break;
        default:
            break;
    }
}

void TribeBox::textFilterChanged()
{
    QRegExp reg_exp(_filter->text(),
                    _filter->caseSensitivity(),
                    _filter->patternSyntax());
    _proxy->setFilterRegExp(reg_exp);
    _proxy->setSelection(_filter->selection());
    _filter->setFound(_proxy->rowCount() != 0 && _model->rowCount() != 0);
}

