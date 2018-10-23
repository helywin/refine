//
// Created by jiang.wenqiang on 2018/9/20.
//

#ifndef REFINE_FRAMEVIEWER_HPP
#define REFINE_FRAMEVIEWER_HPP

#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QStatusBar>

/*!
 * @brief 报文文件查看器
 */

class FrameViewer : public QMainWindow
{
Q_OBJECT
private:
    QMenuBar *_menubar;
    QMenu *_menu_file;
    QAction *_menu_file_open;
    QAction *_menu_file_exit;
    QMenu *_menu_option;
    QAction *_menu_option_frame;
    QWidget *_widget_central;
    QHBoxLayout *_layout_central;
    QTableWidget *_table_central;
    QStatusBar *_statusbar;
    QFileDialog *_file_dialog;
    QString _last_name;

public:
    explicit FrameViewer(QWidget *parent = nullptr);
private:
    void setup();

    inline void initMenu(QMenu *&menu, QString &&title,
                         QMenuBar *m)
    {
        menu = new QMenu(title, m);
        m->addMenu(menu);
    }

    inline void initMenu(QMenu *&menu, QString &&title,
                         QMenu *m)
    {
        menu = new QMenu(title, m);
        m->addMenu(menu);
    }

    void initMenu(QAction *&action, QString &&title,
                  QMenu *m, const QString &tips,
                  bool checkable = false,
                  bool checked = false)
    {
        action = new QAction(title, m);
        m->addAction(action);
        action->setStatusTip(tips);
        action->setCheckable(checkable);
        action->setChecked(checked);
    }

    inline void initMenu(QAction *&action, QString &&title,
                         QMenu *m, const QString &tips,
                         const QKeySequence &seq,
                         bool checkable = false,
                         bool checked = false)
    {
        initMenu(action, qMove(title), m, tips, checkable, checked);
        action->setShortcut(seq);
    }
public:
    void readFrameData(const QString &fname);

    void showOnlyFrame777();
};


#endif //REFINE_FRAMEVIEWER_HPP
