//
// Created by jiang.wenqiang on 2019/1/4.
//

#include "Manual.hpp"

Manual::Manual(QWidget *parent) :
        QDialog(parent)
{
    setup();
}

void Manual::setup()
{
    setWindowTitle(tr("手册"));
    _layout = new QVBoxLayout(this);
    setLayout(_layout);
    _splitter = new QSplitter(this);
    _index = new QTreeView(_splitter);
    _doc = new QTextBrowser(_splitter);
    _layout->addWidget(_splitter);
    _splitter->addWidget(_index);
    _splitter->addWidget(_doc);
}
