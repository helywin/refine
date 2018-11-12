//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_CURVEBOX_HPP
#define REFINE_CURVEBOX_HPP

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtCore/QItemSelectionModel>
#include "TribeModel.hpp"

class CurvePanel;
class TribeView;
class Tribe;
class Sketch;

class CurveBox : public QDockWidget
{
Q_OBJECT
private:
//    CurvePanel *_curve_panel;
    TribeModel *_model;
    TribeView *_view;
    QItemSelectionModel *_selection;
    Tribe *_tribe;
    QHeaderView *_h_header;
    QHeaderView *_v_header;

public:
    explicit CurveBox(Tribe *tribe, QWidget *parent = nullptr);

    inline TribeModel& tribeModel() { return *_model; }

    void connectModelToSketch(Sketch *sketch);

public slots:
    inline void updateData() { _model->genData(_tribe); };

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setup();
};


#endif //REFINE_CURVEBOX_HPP
