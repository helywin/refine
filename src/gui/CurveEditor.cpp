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
    _menubar = new QMenuBar(this);
    _menu_file = new QMenu(tr("文件(&F)"), _menubar);
    _menu_file_import = new QAction(tr("打开(&O)..."), _menu_file);
    _menu_file_import->setShortcut(QKeySequence("Ctrl+O"));
    _menu_file_import->setStatusTip(tr("打开曲线配置文件"));
    _menu_file_export = new QAction(tr("保存(&S)..."), _menu_file);
    _menu_file_export->setShortcut(QKeySequence("Ctrl+S"));
    _menu_file_export->setStatusTip(tr("保存曲线配置文件"));
    _menu_file->addAction(_menu_file_import);
    _menu_file->addAction(_menu_file_export);
    _menu_edit = new QMenu(tr("编辑(&E)"), _menubar);

    _menu_edit_insert = new QAction(tr("插入(&I)"), _menu_edit);
    _menu_edit_insert->setShortcut(QKeySequence("Ctrl+I"));
    _menu_edit_insert->setShortcutVisibleInContextMenu(true);
    _menu_edit_insert->setStatusTip(tr("在当前行前插入行"));

    _menu_edit_append = new QAction(tr("追加(&A)"), _menu_edit);
    _menu_edit_append->setShortcut(QKeySequence("Ctrl+A"));
    _menu_edit_append->setShortcutVisibleInContextMenu(true);
    _menu_edit_append->setStatusTip(tr("在当前行后追加行"));

    _menu_edit_remove = new QAction(tr("删除(&D)"), _menu_edit);
    _menu_edit_remove->setShortcut(QKeySequence("delete"));
    _menu_edit_remove->setShortcutVisibleInContextMenu(true);
    _menu_edit_remove->setStatusTip(tr("删除当前选中行"));

    _menu_edit->addAction(_menu_edit_insert);
    _menu_edit->addAction(_menu_edit_append);
    _menu_edit->addAction(_menu_edit_remove);
    _menu_view = new QMenu(tr("视图(&V)"), _menubar);
    _menu_view_froze = new QAction(tr("冻结(&F)"), _menu_view);
    _menu_edit_remove->setStatusTip(tr("是否冻结名称一列"));
    _menu_view_froze->setCheckable(true);
    _menu_view_froze->setChecked(true);
    _menu_view->addAction(_menu_view_froze);

    _menubar->addMenu(_menu_file);
    _menubar->addMenu(_menu_edit);
    _menubar->addMenu(_menu_view);

    _statusbar = new QStatusBar(this);


    _layout = new QVBoxLayout(this);
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setMargin(0);
    _h_header = new QHeaderView(Qt::Orientation::Horizontal);
    _v_header = new QHeaderView(Qt::Orientation::Vertical);
    _curve_model = new CurveModel(this);
    _curve_model->genData(_curve);
    _h_header->setModel(_curve_model);
    _v_header->setModel(_curve_model);
    _selection = new QItemSelectionModel(_curve_model);
    _curve_view = new CurveView(_curve_model, _selection, _h_header, _v_header, this);
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
    _curve_view->setItemDelegateForColumn(CurveModel::UnitColumn, _unit);
    _curve_view->setItemDelegateForColumn(CurveModel::WidthColumn, _line_width);
    _curve_view->setItemDelegateForColumn(CurveModel::ColorColumn, _color);
    _curve_view->setItemDelegateForColumn(CurveModel::LowByteColumn, _low_byte_input);
    _curve_view->setItemDelegateForColumn(CurveModel::HighByteColumn,
                                          _high_byte_input);
    _curve_view->setItemDelegateForColumn(CurveModel::CanIdColumn, _can_id);
    _h_header->setParent(_curve_view);
    _v_header->setParent(_curve_view);
    setLayout(_layout);
    _layout->setMenuBar(_menubar);
    _layout->addWidget(_curve_view);
    _layout->addWidget(_statusbar);
    _curve_view->setRightMenu(_menu_edit);
    connect(_menu_view_froze, &QAction::triggered,
            this, &CurveEditor::setFroze, Qt::DirectConnection);
    connect(_menu_edit_insert, &QAction::triggered,
            this, &CurveEditor::insertCurveRow, Qt::DirectConnection);
    connect(_menu_edit_append, &QAction::triggered,
            this, &CurveEditor::appendCurveRow, Qt::DirectConnection);
    connect(_menu_edit_remove, &QAction::triggered,
            this, &CurveEditor::removeCurveRow, Qt::DirectConnection);
}

void CurveEditor::insertCurveRow()
{
    _curve_model->insertRow(_selection->currentIndex().row());
}

void CurveEditor::appendCurveRow()
{
    _curve_model->appendRow(_selection->currentIndex().row());
}

void CurveEditor::removeCurveRow()
{
    _curve_model->removeRow(_selection->currentIndex().row());
}
