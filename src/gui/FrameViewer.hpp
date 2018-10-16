//
// Created by jiang.wenqiang on 2018/9/20.
//

#ifndef REFINE_FRAMEVIEWER_HPP
#define REFINE_FRAMEVIEWER_HPP

#include <QtCore/QFile>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QFileDialog>

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
    QAction *_menu_option_frame777;
    QMenu *_menu_help;
    QAction *_menu_help_about;
    QWidget *_widget_central;
    QHBoxLayout *_layout_central;
    QTableWidget *_table_central;
    QFileDialog *_file_dialog;
    QString _last_name;

public:
    explicit FrameViewer(QWidget *parent = nullptr);
private:
    void setupUi();

public:
    void readFrameData(const QString &fname);

    void showOnlyFrame777();
};


#endif //REFINE_FRAMEVIEWER_HPP
