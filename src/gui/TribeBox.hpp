//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_CURVEBOX_HPP
#define REFINE_CURVEBOX_HPP

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtCore/QItemSelectionModel>
#include <QtCore/QStringListModel>
#include "TribeModel.hpp"
#include "TribeSortModel.hpp"
#include "Message.hpp"
#include "CompleteModel.hpp"

class CurvePanel;
class TribeView;
class Tribe;
class Sketch;
class TribeFilter;

class TribeBox : public QDockWidget, public Message
{
Q_OBJECT
private:
    TribeView *_view;
    QWidget *_content;
    QCheckBox *_check;
    QWidget *_search;
    QVBoxLayout *_layout_content;
    QHBoxLayout *_layout_search;
    QItemSelectionModel *_selection;
    Tribe *_tribe;
    QHeaderView *_h_header;
    QHeaderView *_v_header;
//    CurvePanel *_curve_panel;
    TribeModel *_model;
    TribeSortModel *_proxy;
    TribeFilter *_filter;
    Sketch *_sketch;
    CompleteModel *_complete_model;

public:
    explicit TribeBox(Tribe *tribe, Message *message = nullptr,
                      QWidget *parent = nullptr);

    inline TribeModel &tribeModel() { return *_model; }

    void connectModelToSketch(Sketch *sketch);

    inline QWidget *content() { return _content; }

public slots:

    inline void updateData() { _model->genData(_tribe); };

private:
    void setup();
private slots:
    void selectionChanged(const QItemSelection &selected,
                          const QItemSelection &deselected);

    void setDisplayItem(int state);

    void textFilterChanged();
};


#endif //REFINE_CURVEBOX_HPP
