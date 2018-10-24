//
// Created by jiang.wenqiang on 2018/10/23.
//

#include <QtWidgets/QLabel>
#include "Output.hpp"
#include "Messager.hpp"
#include "Refine.hpp"

Output::Output(QWidget *parent) :
        QDockWidget(parent)
{
    setup();
}

void Output::setup()
{
    setWindowTitle(tr("信息"));
    _messager = new Messager(this);
    _content = new QWidget();
    _content->resize(200, 200);
    _layout = new QHBoxLayout(_content);
    _content->setLayout(_layout);
    _layout->setContentsMargins(5, 0, 0, 0);
    setWidget(_content);
    _layout->addWidget(_messager);
}

void Output::connectToMessager(Refine *emitter)
{
    connect(emitter, &Refine::message, _messager,
            &Messager::showMessage, Qt::DirectConnection);
}

void Output::connectToMessager(Revolve *emitter)
{
    connect(emitter, &Revolve::message, _messager,
            &Messager::showMessage, Qt::DirectConnection);
}