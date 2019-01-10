//
// Created by jiang.wenqiang on 2018/11/16.
//

#include <QtWidgets/QWidgetAction>
#include <QtGui/QPainter>
#include "TribeFilter.hpp"
#include "Style.hpp"

TribeFilter::TribeFilter(TribeModel *model, QWidget *parent) :
        QLineEdit(parent),
        _is_found(false),
        _model(model)
{
    setup();
}

void TribeFilter::setup()
{
    setClearButtonEnabled(true);
    setFont(St::font_regular);
    _menu = new QMenu(this);
    _case_sensitive = new QAction(tr("区分大小写(&C)"), _menu);
    _case_sensitive->setCheckable(true);
    _case_sensitive->setStatusTip(tr("查找是否区分输入大小写"));
    _curve_selection = new QActionGroup(this);
    _checked_items = new QAction(tr("选中(&S)"), _curve_selection);
    _checked_items->setData(QVariant(int(Tribe::Selection::SelectChecked)));
    _checked_items->setStatusTip(tr("只在打钩的项目中查找"));
    _unchecked_items = new QAction(tr("未选中(&U)"), _curve_selection);
    _unchecked_items->setData(QVariant(int(Tribe::Selection::SelectUnchecked)));
    _unchecked_items->setStatusTip(tr("只在未打钩的项目中查找"));
    _all_items = new QAction(tr("所有(&A)"), _curve_selection);
    _all_items->setData(QVariant(int(Tribe::Selection::SelectAll)));
    _all_items->setStatusTip(tr("在所有项目中查找"));
    _checked_items->setCheckable(true);
    _unchecked_items->setCheckable(true);
    _all_items->setCheckable(true);
    _all_items->setChecked(true);
    _pattern = new QActionGroup(this);
    _fix_string = new QAction(tr("固定字符串(&F)"), _pattern);
    _fix_string->setData(QVariant(int(QRegExp::FixedString)));
    _fix_string->setStatusTip(tr("根据固定字符串匹配"));
    _regexp = new QAction(tr("正则表达式(&R)"), _pattern);
    _regexp->setData(QVariant(int(QRegExp::RegExp2)));
    _regexp->setStatusTip(tr("根据正则表达式匹配"));
    _wildcard = new QAction(tr("通配符(&W)"), _pattern);
    _wildcard->setData(QVariant(int(QRegExp::Wildcard)));
    _wildcard->setStatusTip(tr("根据通配符匹配"));
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
    _filter_btn->setIcon(QIcon(":res/icons/search.ico"));
    _filter_btn->setFocusPolicy(Qt::NoFocus);
//    _filter_btn->setStyleSheet("* { border: none; }, menu-indicator {image: none;}");
    _filter_btn->setStyleSheet("QToolButton {border: none;} "
                               "QToolButton::menu-indicator {image: none;}");
    _filter_btn->setPopupMode(QToolButton::InstantPopup);
    _filter_btn->setStatusTip(tr("弹出搜索配置菜单"));
    auto *options_action = new QWidgetAction(this);
    options_action->setDefaultWidget(_filter_btn);
    addAction(options_action, QLineEdit::LeadingPosition);
    _completer.setCaseSensitivity(Qt::CaseInsensitive);
    setCompleter(&_completer);


    connect(_case_sensitive, &QAction::triggered,
            this, &TribeFilter::changeCaseSensitive, Qt::DirectConnection);
    connect(_curve_selection, &QActionGroup::triggered,
            this, &TribeFilter::changeSelection, Qt::DirectConnection);
    connect(_pattern, &QActionGroup::triggered,
            this, &TribeFilter::changePatternSyntax, Qt::DirectConnection);
    connect(this, &TribeFilter::textChanged,
            this, &TribeFilter::filterChanged, Qt::DirectConnection);
}

void TribeFilter::paintEvent(QPaintEvent *event)
{
//    _font.setStrikeOut(!_is_found);
    QLineEdit::paintEvent(event);
//    _font.setStrikeOut(false);
//    setFont(_font);
    QPainter painter;
    painter.begin(this);
    if (!hasFocus() && text().isEmpty()) {
        painter.setFont(St::font_regular);
        painter.setPen(St::color_hint);
        painter.drawText(QRect(rect().left() + rect().height(),
                               rect().top(),
                               rect().width(),
                               rect().height()),
                         tr("←筛选，搜索"),
                         QTextOption(Qt::AlignVCenter | Qt::AlignLeft));
    }
    painter.end();
//    _font.setStrikeOut(!_is_found);
//    setFont(_font);
}

void TribeFilter::setFound(bool is_found)
{
    Q_UNUSED(is_found);
//    _is_found = is_found;
//    _font.setStrikeOut(!_is_found);
//    setFont(_font);
//    update();
}

Qt::CaseSensitivity TribeFilter::caseSensitivity() const
{
    return _case_sensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
}

void TribeFilter::setCaseSensitivity(Qt::CaseSensitivity cs)
{
    _case_sensitive->setChecked(cs == Qt::CaseSensitive);
}

QRegExp::PatternSyntax TribeFilter::patternSyntax() const
{
    return static_cast<QRegExp::PatternSyntax>(_pattern->checkedAction()->data().toInt());
}

void TribeFilter::setPatternSyntax(QRegExp::PatternSyntax s)
{
    for (auto *action : _pattern->actions()) {
        if (action->data().toInt() == s) {
            action->setChecked(true);
            break;
        }
    }
}

Tribe::Selection TribeFilter::selection() const
{
    return static_cast<Tribe::Selection>(_curve_selection->checkedAction()->data().toInt());
}

void TribeFilter::setSelection(Tribe::Selection selection)
{
    for (auto *action : _curve_selection->actions()) {
        if (action->data().toInt() == selection) {
            action->setChecked(true);
            break;
        }
    }
    emit filterChanged();
}

void TribeFilter::changeCaseSensitive()
{
    _completer.setCaseSensitivity(_case_sensitive->isChecked() ?
                                  Qt::CaseSensitive : Qt::CaseInsensitive);
    emit filterChanged();
}

void TribeFilter::changeSelection(QAction *action)
{
    _complete_model->setSelection(
            static_cast<Tribe::Selection>(action->data().toInt()));
    _complete_model->genData();
    emit filterChanged();
}

void TribeFilter::changePatternSyntax(QAction *action)
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
