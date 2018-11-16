//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_CURVEBOX_HPP
#define REFINE_CURVEBOX_HPP

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QItemSelectionModel>
#include "TribeModel.hpp"
#include "TribeSortModel.hpp"
#include "Message.hpp"

class CurvePanel;
class TribeView;
class Tribe;
class Sketch;
class CurveFilter;

class CurveBox : public QDockWidget, public Message
{
Q_OBJECT
private:
    TribeView *_view;
    QWidget *_content;
    QVBoxLayout *_layout;
    QItemSelectionModel *_selection;
    Tribe *_tribe;
    QHeaderView *_h_header;
    QHeaderView *_v_header;
//    CurvePanel *_curve_panel;
    TribeModel *_model;
    TribeSortModel *_proxy;
    CurveFilter *_filter;
    Sketch *_sketch;

public:
    explicit CurveBox(Tribe *tribe, Message *message = nullptr,
                      QWidget *parent = nullptr);

    inline TribeModel &tribeModel() { return *_model; }

    void connectModelToSketch(Sketch *sketch);

public slots:

    inline void updateData() { _model->genData(_tribe); };

private:
    void setup();
private slots:
    void selectionChanged(const QItemSelection &selected,
                          const QItemSelection &deselected);
};


#endif //REFINE_CURVEBOX_HPP
