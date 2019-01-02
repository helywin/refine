//
// Created by jiang.wenqiang on 2019/1/2.
//

#include <QtGui/QKeyEvent>
#include "Command.hpp"

Command::Command(QWidget *parent) : QLineEdit(parent)
{
    setup();
}

void Command::commandChange(const QString &string)
{
    QString str = string;
    if (_upper_cased) {
        str = str.toUpper();
    }
    setText(str);
    emit commandChanged(str);
}

void Command::setup()
{
    connect(this, &Command::textChanged, this, &Command::commandChange);
}

void Command::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            if (!text().isEmpty()) {
                emit commandConfirmed(text());
                _index = -1;
                if (_sequence.contains(text())) {
                    _sequence.move(_sequence.indexOf(text()), _sequence.size() - 1);
                } else {
                    _sequence.append(text());
                }
                if (_clear_flag) {
                    setText(QString());
                }
                event->accept();
            }
            break;
        case Qt::Key_Escape:
            setText(QString());
            event->accept();
            break;
        case Qt::Key_Up:
        case Qt::Key_PageUp:
            if (_index == -1) {
                if (!_sequence.isEmpty()) {
                    _index = _sequence.size() - 1;
                    setText(_sequence.at(_index));
                }
            } else {
                if (_index > 0) {
                    _index -= 1;
                    setText(_sequence.at(_index));
                }
            }
            event->accept();
            break;
        case Qt::Key_Down:
        case Qt::Key_PageDown:
            if (_index == -1) {
                if (!_sequence.isEmpty()) {
                    _index = _sequence.size() - 1;
                    setText(_sequence.at(_index));
                }
            } else {
                if (_index < _sequence.size() - 1) {
                    _index += 1;
                    setText(_sequence.at(_index));
                }
            }
            event->accept();
            break;
        default:
            break;
    }
    QLineEdit::keyPressEvent(event);
}
