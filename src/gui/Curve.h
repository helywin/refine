//
// Created by jiang.wenqiang on 2018/7/23.
//

#ifndef REFINE_CURVE_H
#define REFINE_CURVE_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QVBoxLayout>

#include "CurveTable.h"

class Curve : public QDialog {
Q_OBJECT
private:
    QVBoxLayout *layout;

    QMenuBar *menu_bar;
    QMenu *menu_file;
    QMenu *menu_edit;
    QMenu *menu_action;
    QAction *menu_file_open;
    QAction *menu_file_save;
    QAction *menu_file_saveas;
    QAction *menu_edit_reset;
    QAction *menu_action_finish;
    QAction *menu_action_abort;

    CurveTable *table;
public:
    explicit Curve(QWidget *parent);

    ~Curve() final = default;

protected:
    void setup_ui();
};


#endif //REFINE_CURVE_H
