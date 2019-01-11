//
// Created by jiang.wenqiang on 2019/1/2.
//

#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QWidgetAction>
#include "Command.hpp"
#include "Style.hpp"

Command::Command(QWidget *parent) : QLineEdit(parent)
{
    setup();
}

void Command::commandChange(const QString &string)
{
    QString str = string;
    if (_menu_uppercased->isChecked()) {
        str = str.toUpper();
    }
    setText(str);
    emit commandChanged(str);
}

void Command::setup()
{
    setFont(St::font_regular);
    setPrefix(_prefixs);
    _menu = new QMenu(tr("设置"), this);
    _menu_uppercased = new QAction(tr("自动大写(&U)"), _menu);
    _menu_uppercased->setCheckable(true);
    _menu_uppercased->setChecked(true);
    _menu_uppercased->setStatusTip(tr("输入内容自动变为大写"));
    _menu_clear = new QAction(tr("自动清空(&C)"), _menu);
    _menu_clear->setCheckable(true);
    _menu_clear->setChecked(true);
    _menu_clear->setStatusTip(tr("发送成功后自动清除内容"));
    _menu->addAction(_menu_uppercased);
    _menu->addAction(_menu_clear);
    _menu->addSeparator();

    _settings = new QToolButton(this);
    _settings->setCursor(Qt::ArrowCursor);
    _settings->setMenu(_menu);
    _settings->setIcon(QIcon(":res/icons/can.ico"));
    _settings->setFocusPolicy(Qt::NoFocus);
    _settings->setStyleSheet("QToolButton {border: none;} "
                             "QToolButton::menu-indicator {image: none;}");
    _settings->setPopupMode(QToolButton::InstantPopup);
    _settings->setStatusTip(tr("弹出设置菜单"));
    auto *options_action = new QWidgetAction(this);
    options_action->setDefaultWidget(_settings);
    addAction(options_action, QLineEdit::TrailingPosition);

    connect(this, &Command::textChanged, this, &Command::commandChange);
    connect(_menu_uppercased, &QAction::triggered, this, [this](){
        if (_menu_uppercased->isChecked()) {
            setText(text().toUpper());
        }
    });
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
    QFontMetrics metrics(St::font_regular);
    QPen pen_none(QColor(212, 212, 212, 255));
    QPen pen(Qt::black);
    QBrush brush(QColor(212, 212, 212));
    painter.begin(this);
    painter.setFont(St::font_regular);
    QString str = _prefixs.join(';');
    int rect_start = rect().left() + 1;
    int w = metrics.boundingRect(str).width() + 10;
    QRect prefix_rect(rect_start, rect().top() + 1, w, rect().height() - 3);
    painter.setBrush(brush);
    painter.setPen(pen_none);
    painter.drawRect(prefix_rect);
    painter.setPen(pen);
    painter.drawText(prefix_rect, str, QTextOption(Qt::AlignHCenter | Qt::AlignVCenter));
    if (!hasFocus() && text().isEmpty()) {
        painter.setFont(St::font_regular);
        painter.setPen(St::color_hint);
        painter.drawText(QRect(rect().left() + _text_margins.left(),
                               rect().top(),
                               rect().width() - _text_margins.left(),
                               rect().height()),
                         tr("回车发送，↑↓切换历史输入"),
                         QTextOption(Qt::AlignVCenter | Qt::AlignLeft));
    }
    painter.end();
}

void Command::setPrefix(const QStringList &prefix)
{
    _prefixs = prefix;
    QFontMetrics metrics(St::font_regular);
    int w = metrics.boundingRect(_prefixs.join(';')).width();
    _text_margins = QMargins(w + 15, 0, 0, 0);
    setTextMargins(_text_margins);
    update();
}

void Command::clearCommand()
{
    if (_menu_clear->isChecked()) {
        setText(QString());
    }
}

void Command::addCommandActions(QList<QAction *> actions)
{
    _menu->addActions(qMove(actions));
}
