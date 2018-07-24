//
// Created by jiang.wenqiang on 2018/7/23.
//

#ifndef REFINE_SELECT_H
#define REFINE_SELECT_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMenuBar>

class Select : public QDialog {
Q_OBJECT
private:
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

    QHBoxLayout *layout;
    QTreeWidget *whole_modes;
    QListWidget *modes;
    QListWidget *criteria;
    QTableWidget *params;
public:
    explicit Select(QWidget *parent = nullptr);

    ~Select() final = default;

protected:
    void setup_ui();

    void closeEvent(QCloseEvent *event) override;

    void showEvent(QShowEvent *event) override;
};


#endif //REFINE_SELECT_H
