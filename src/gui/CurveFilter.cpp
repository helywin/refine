//
// Created by jiang.wenqiang on 2018/11/16.
//

#include <QtWidgets/QWidgetAction>
#include <QtGui/QPainter>
#include "CurveFilter.hpp"

CurveFilter::CurveFilter(TribeModel *model, QWidget *parent) :
        QLineEdit(parent),
        _is_found(false),
        _model(model)
{
    setup();
}

void CurveFilter::setup()
{
    setClearButtonEnabled(true);
    _font = QFont("微软雅黑", 10);
    setFont(_font);
    _menu = new QMenu(this);
    _case_sensitive = new QAction(tr("区分大小写(&C)"), _menu);
    _case_sensitive->setCheckable(true);
    _curve_items = new QActionGroup(this);
    _checked_items = new QAction(tr("选中(&S)"), _curve_items);
    _checked_items->setData(QVariant(int(Selection::SelectChecked)));
    _unchecked_items = new QAction(tr("未选中(&U)"), _curve_items);
    _unchecked_items->setData(QVariant(int(Selection::SelectUnchecked)));
    _all_items = new QAction(tr("所有(&A)"), _curve_items);
    _all_items->setData(QVariant(int(Selection::SelectAll)));
    _checked_items->setCheckable(true);
    _unchecked_items->setCheckable(true);
    _all_items->setCheckable(true);
    _all_items->setChecked(true);
    _pattern = new QActionGroup(this);
    _fix_string = new QAction(tr("固定字符串(&F)"), _pattern);
    _fix_string->setData(QVariant(int(QRegExp::FixedString)));
    _regexp = new QAction(tr("正则表达式(&R)"), _pattern);
    _regexp->setData(QVariant(int(QRegExp::RegExp2)));
    _wildcard = new QAction(tr("通配符(&W)"), _pattern);
    _wildcard->setData(QVariant(int(QRegExp::Wildcard)));
    _fix_string->setCheckable(true);
    _regexp->setCheckable(true);
    _wildcard->setCheckable(true);
    _wildcard->setChecked(true);
    _menu->addAction(_case_sensitive);
    _menu->addSeparator();
    _menu->addActions(_curve_items->actions());
    _menu->addSeparator();
    _menu->addActions(_pattern->actions());
    _filter_btn = new QToolButton(this);
    _filter_btn->setCursor(Qt::ArrowCursor);
    _filter_btn->setMenu(_menu);
    _filter_btn->setIcon(QIcon(":res/icons/search.png"));
    _filter_btn->setFocusPolicy(Qt::NoFocus);
//    _filter_btn->setStyleSheet("* { border: none; }, menu-indicator {image: none;}");
    _filter_btn->setStyleSheet("QToolButton {border: none;} "
                               "QToolButton::menu-indicator {image: none;}");
    _filter_btn->setPopupMode(QToolButton::InstantPopup);
    auto *options_action = new QWidgetAction(this);
    options_action->setDefaultWidget(_filter_btn);
    addAction(options_action, QLineEdit::LeadingPosition);
    _completer.setCaseSensitivity(Qt::CaseInsensitive);
//    _completer.setModel(_model);
    setCompleter(&_completer);
}

void CurveFilter::paintEvent(QPaintEvent *event)
{
    QLineEdit::paintEvent(event);
    QPainter painter;
    painter.begin(this);
    if (!hasFocus() && text().isEmpty()) {
        QPen pen(QColor(149, 149, 149));
        painter.setFont(_font);
        painter.drawText(QRect(rect().left() + rect().height(),
                               rect().top(),
                               rect().width(),
                               rect().height()),
                         tr("输入内容进行搜索"),
                         QTextOption(Qt::AlignVCenter | Qt::AlignLeft));
    }
    painter.end();
}

void CurveFilter::setFound(bool is_found)
{
    _is_found = is_found;
    if (_is_found) {
        setStyleSheet("QLineEdit { background: #ffffff;}");
    } else {
        setStyleSheet("QLineEdit { background: rgb(255, 204, 204);}");
    }
}
