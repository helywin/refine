//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_CURVEEDITOR_HPP
#define REFINE_CURVEEDITOR_HPP


#include <QtWidgets/QDialog>
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
    CurveView *_view;
    CurveModel *_model;
    QItemSelectionModel *_selection;
    QVBoxLayout *_layout;
    Curve *_curve;
    QHeaderView *_h_header;
    QHeaderView *_v_header;
    SpecInput *_line_width;
    SpecInput *_can_id;
    ColorInput *_color;
    SpecInput *_frame_msec;
    ByteInput *_low_byte_input;
    ByteInput *_high_byte_input;
    ComboInput *_unit;


public:
    explicit CurveEditor(Curve *curve, QWidget *parent = nullptr);

private:
    void setup();

public slots:
    inline void updateData() { _model->genData(_curve); };

};


#endif //REFINE_CURVEEDITOR_HPP
