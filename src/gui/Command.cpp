//
// Created by jiang.wenqiang on 2019/1/2.
//

#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
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
    setFont(_font);
    setPrefix(_prefixs);
    connect(this, &Command::textChanged, this, &Command::commandChange);
}

void Command::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return: {
            QString str = text();
            if (!str.isEmpty()) {
                emit commandConfirmed(str);
                _index = -1;
                if (_sequence.contains(str)) {
                    _sequence.move(_sequence.indexOf(str), _sequence.size() - 1);
                } else {
                    _sequence.append(str);
                }
                event->accept();
            }
            break;
        }
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

void Command::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);
    QPainter painter;
    QFontMetrics metrics(_font);
    QPen pen_none(QColor(212, 212, 212, 255));
    QPen pen(Qt::black);
    QBrush brush(QColor(212, 212, 212));
    painter.begin(this);
    painter.setFont(_font);
    QString str = _prefixs.join(';');
    int rect_start = rect().left() + 1;
    int w = metrics.boundingRect(str).width() + 10;
    QRect prefix_rect(rect_start, rect().top() + 1, w, rect().height() - 3);
    painter.setBrush(brush);
    painter.setPen(pen_none);
    painter.drawRect(prefix_rect);
    painter.setPen(pen);
    painter.drawText(prefix_rect, str, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
    painter.end();
}

void Command::setPrefix(const QStringList &prefix)
{
    _prefixs = prefix;
    QFontMetrics metrics(_font);
    int w = metrics.boundingRect(_prefixs.join(';')).width();
    setTextMargins(w + 15, 0, 0, 0);
    update();
}
