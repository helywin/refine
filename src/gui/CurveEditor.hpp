//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_CURVEEDITOR_HPP
#define REFINE_CURVEEDITOR_HPP


#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>
#include "CurveView.hpp"
#include "CurveModel.hpp"

class CurveEditor : public QDialog
{
Q_OBJECT
private:
    CurveView *_view;
    CurveModel *_model;
    QItemSelectionModel *_selection;
    QVBoxLayout *_layout;
    Curve *_curve;

public:
    explicit CurveEditor(Curve *curve, QWidget *parent = nullptr);

private:
    void setup();

public slots:
    inline void updateData() { _model->genData(_curve); };

};


#endif //REFINE_CURVEEDITOR_HPP
