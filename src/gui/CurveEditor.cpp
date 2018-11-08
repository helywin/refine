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
    setWindowFlag(Qt::WindowMinMaxButtonsHint, true);
    resize(800, 600);
    _layout = new QVBoxLayout(this);
    _layout->setContentsMargins(0, 0, 0, 0);
    _h_header = new QHeaderView(Qt::Orientation::Horizontal);
    _v_header = new QHeaderView(Qt::Orientation::Vertical);
    _model = new CurveModel(this);
    _h_header->setModel(_model);
    _v_header->setModel(_model);
    _selection = new QItemSelectionModel(_model);
    _view = new CurveView(_model, _selection, _h_header, _v_header, this);
    _line_width = new SpecInput(1, this);
    _low_byte_input = new ByteInput(false, this);
    _high_byte_input = new ByteInput(true, this);
    _can_id = new SpecInput(3, this, 16);
    _color = new ColorInput(this);
    _unit = new ComboInput(
            true,
            QStringList({"NULL", "mA", "A", "mV", "V", "rpm", "mm", "cm", "m",
                         "m/s", "m/s²", "On/Off", "Status", "kph", "Hz", "KHz",
                         "bar", "%", "rad/s", "Nm", "N", "℃"}),
            this);
    _view->setItemDelegateForColumn(CurveModel::UnitColumn, _unit);
    _view->setItemDelegateForColumn(CurveModel::WidthColumn, _line_width);
    _view->setItemDelegateForColumn(CurveModel::ColorColumn, _color);
    _view->setItemDelegateForColumn(CurveModel::LowByteColumn, _low_byte_input);
    _view->setItemDelegateForColumn(CurveModel::HighByteColumn,
                                    _high_byte_input);
    _view->setItemDelegateForColumn(CurveModel::CanIdColumn, _can_id);
    _h_header->setParent(_view);
    _v_header->setParent(_view);
    setLayout(_layout);
    _layout->addWidget(_view);
}
