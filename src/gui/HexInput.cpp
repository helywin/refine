//
// Created by jiang.wenqiang on 2019/1/7.
//

#include <QtGui/QPainter>
#include "HexInput.hpp"

HexInput::HexInput(int num, QWidget *parent) :
        QLineEdit(parent),
        _num(num)
{
    setup();
}

void HexInput::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);
    QPainter painter(this);
    painter.setFont(_font);
    painter.drawText(QRect(rect().left() + 4, rect().top(), 18, rect().height()),
                     "0x", QTextOption(Qt::AlignVCenter | Qt::AlignLeft));
    painter.end();
}

void HexInput::setup()
{
    _font = QFont("微软雅黑", 10);
    setClearButtonEnabled(true);
    setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
//    QString mask = QString("H").repeated(_num);
    _validator = new QRegExpValidator(QRegExp(QString("[0-9a-fA-F]{0,%1}").arg(_num)), this);
    setValidator(_validator);
//    setInputMask(mask);
    QMargins margins = textMargins();
    margins.setLeft(18);
    setTextMargins(margins);
}

void HexInput::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
    selectAll();
}
