//
// Created by jiang.wenqiang on 2018/7/23.
//

#ifndef REFINE_CURVE_H
#define REFINE_CURVE_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QVBoxLayout>

#include "CurveTable.h"
#include "TableTips.h"

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
    QAction *menu_edit_add;
    QAction *menu_edit_insert;
    QAction *menu_edit_delete;
protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QAction *menu_action_finish;
    QAction *menu_action_abort;

    CurveTable *table;
public:
    explicit Curve(QWidget *parent);

    ~Curve() final = default;

protected:
    void setupUi();
};


#endif //REFINE_CURVE_H
