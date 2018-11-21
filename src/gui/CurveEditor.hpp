//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_CURVEEDITOR_HPP
#define REFINE_CURVEEDITOR_HPP


#include <QtWidgets/QDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QStyledItemDelegate>
#include "CurveView.hpp"
#include "CurveModel.hpp"
#include "SpecInput.hpp"
#include "ByteInput.hpp"
#include "ComboInput.hpp"
#include "ColorInput.hpp"

class CurveEditor : public QDialog
{
Q_OBJECT
private:
    QMenuBar *_menubar;
    QMenu *_menu_file;
    QAction *_menu_file_import;
    QAction *_menu_file_export;
    //右键菜单
    QMenu *_menu_edit;
    QAction *_menu_edit_insert;
    QAction *_menu_edit_remove;
    QAction *_menu_edit_append;

    QMenu *_menu_view;
    QAction *_menu_view_froze;

    QStatusBar *_statusbar;
    CurveView *_curve_view;
    CurveModel *_curve_model;
    QItemSelectionModel *_selection;
    QVBoxLayout *_layout;
    Curve *_curve;
    QHeaderView *_h_header;
    QHeaderView *_v_header;
    SpecInput *_line_width;
    SpecInput *_can_id;
    ColorInput *_color;
    ByteInput *_low_byte_input;
    ByteInput *_high_byte_input;
    ComboInput *_unit;


public:
    explicit CurveEditor(Curve *curve, QWidget *parent = nullptr);

    inline QAction *actionImport() { return _menu_file_import; }

    inline QAction *actionExport() { return _menu_file_export; }

private:
    void setup();

public slots:

    inline void updateData() { _curve_model->genData(_curve); };

private slots:

    inline void setFroze() { _curve_view->setFroze(_menu_view_froze->isChecked()); };

    void insertCurveRow();

    void appendCurveRow();

    void removeCurveRow();

};


#endif //REFINE_CURVEEDITOR_HPP
