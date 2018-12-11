//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_TRIBEBOX_HPP
#define REFINE_TRIBEBOX_HPP

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
#include "SpecInput.hpp"
#include "SpinInput.hpp"
#include "ComboInput.hpp"

class CurvePanel;
class TribeView;
class Tribe;
class Sketch;
class SketchY;
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
    SketchY *_sketch_y;
    CompleteModel *_complete_model;
    SpinInput *_width;
    SpinInput *_precision;
    ComboInput *_unit;

public:
    explicit TribeBox(Tribe *tribe, Message *message = nullptr,
                      QWidget *parent = nullptr);

    inline TribeModel &tribeModel() { return *_model; }

    void connectModelToSketch(Sketch *sketch);

    void connectModelToSketchY(SketchY *sketch_y);

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


#endif //REFINE_TRIBEBOX_HPP
