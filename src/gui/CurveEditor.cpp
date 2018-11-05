//
// Created by jiang.wenqiang on 2018/10/23.
//

#include "CurveEditor.hpp"

CurveEditor::CurveEditor(Curve *curve, QWidget *parent) :
        _curve(curve),
        QDialog(parent)
{
    setup();
}

void CurveEditor::setup()
{
    setWindowTitle(tr("曲线配置编辑"));
    resize(800, 600);
    _layout = new QVBoxLayout(this);
    _view = new CurveView(this);
    _model = new CurveModel(this);
    _view->setModel(_model);
    _selection = new QItemSelectionModel(_model);
    _view->setSelectionModel(_selection);
    setLayout(_layout);
    _layout->addWidget(_view);
    _view->setHorizontalScrollMode(QTableView::ScrollPerPixel);
    _view->setVerticalScrollMode(QTableView::ScrollPerPixel);
}
