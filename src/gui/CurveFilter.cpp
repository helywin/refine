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
    _curve_selection = new QActionGroup(this);
    _checked_items = new QAction(tr("选中(&S)"), _curve_selection);
    _checked_items->setData(QVariant(int(Tribe::Selection::SelectChecked)));
    _unchecked_items = new QAction(tr("未选中(&U)"), _curve_selection);
    _unchecked_items->setData(QVariant(int(Tribe::Selection::SelectUnchecked)));
    _all_items = new QAction(tr("所有(&A)"), _curve_selection);
    _all_items->setData(QVariant(int(Tribe::Selection::SelectAll)));
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
    _menu->addActions(_curve_selection->actions());
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
    setCompleter(&_completer);


    connect(_case_sensitive, &QAction::triggered,
            this, &CurveFilter::changeCaseSensitive, Qt::DirectConnection);
    connect(_curve_selection, &QActionGroup::triggered,
            this, &CurveFilter::changeSelection, Qt::DirectConnection);
    connect(_pattern, &QActionGroup::triggered,
            this, &CurveFilter::changePatternSyntax, Qt::DirectConnection);
    connect(this, &CurveFilter::textChanged,
            this, &CurveFilter::filterChanged, Qt::DirectConnection);
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

Qt::CaseSensitivity CurveFilter::caseSensitivity() const
{
    return _case_sensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
}

void CurveFilter::setCaseSensitivity(Qt::CaseSensitivity cs)
{
    _case_sensitive->setChecked(cs == Qt::CaseSensitive);
}

QRegExp::PatternSyntax CurveFilter::patternSyntax() const
{
    return static_cast<QRegExp::PatternSyntax>(_pattern->checkedAction()->data().toInt());
}

void CurveFilter::setPatternSyntax(QRegExp::PatternSyntax s)
{
    for (auto *action : _pattern->actions()) {
        if (action->data().toInt() == s) {
            action->setChecked(true);
            break;
        }
    }
}

Tribe::Selection CurveFilter::selection() const
{
    return static_cast<Tribe::Selection>(_curve_selection->checkedAction()->data().toInt());
}

void CurveFilter::setSelection(Tribe::Selection selection)
{
    for (auto *action : _curve_selection->actions()) {
        if (action->data().toInt() == selection) {
            action->setChecked(true);
            break;
        }
    }
    emit filterChanged();
}

void CurveFilter::changeCaseSensitive()
{
    _completer.setCaseSensitivity(_case_sensitive->isChecked() ?
                                  Qt::CaseSensitive : Qt::CaseInsensitive);
    emit filterChanged();
}

void CurveFilter::changeSelection(QAction *action)
{
    _complete_model->setSelection(
            static_cast<Tribe::Selection>(action->data().toInt()));
    _complete_model->genData();
    emit filterChanged();
}

void CurveFilter::changePatternSyntax(QAction *action)
{
    switch (action->data().toInt()) {
        case QRegExp::RegExp2:
            setCompleter(nullptr);
            break;
        case QRegExp::FixedString:
        case QRegExp::Wildcard:
        default:
            setCompleter(&_completer);
            break;
    }
    emit filterChanged();
}
