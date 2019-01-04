//
// Created by jiang.wenqiang on 2019/1/4.
//

#ifndef REFINE_MANUAL_HPP
#define REFINE_MANUAL_HPP

#include <QtWidgets/QDialog>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTextBrowser>

class Manual : public QDialog
{
Q_OBJECT
private:
    QVBoxLayout *_layout;
    QSplitter *_splitter;
    QTreeView *_index;
    QTextBrowser *_doc;

public:
    explicit Manual(QWidget *parent);

private:
    void setup();
};


#endif //REFINE_MANUAL_HPP
