//
// Created by jiang.wenqiang on 2018/11/16.
//

#ifndef REFINE_CURVEFILTER_HPP
#define REFINE_CURVEFILTER_HPP

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QAction>
#include <QtWidgets/QActionGroup>
#include <QtWidgets/QMenu>
#include <QtCore/QRegExp>
#include <QtWidgets/QCompleter>
#include <QtCore/QStringListModel>
#include "CompleteModel.hpp"

class TribeModel;

class CurveFilter : public QLineEdit
{
Q_OBJECT
    Q_PROPERTY(Qt::CaseSensitivity caseSensitivity
                       READ
                       caseSensitivity
                       WRITE
                       setCaseSensitivity)
    Q_PROPERTY(QRegExp::PatternSyntax patternSyntax
                       READ
                       patternSyntax
                       WRITE
                       setPatternSyntax)
private:
    QMenu *_menu;
    QAction *_case_sensitive;
    QActionGroup *_curve_selection;
    QAction *_checked_items;
    QAction *_unchecked_items;
    QAction *_all_items;
    QActionGroup *_pattern;
    QAction *_fix_string;
    QAction *_regexp;
    QAction *_wildcard;
    QToolButton *_filter_btn;
    QFont _font;
    QCompleter _completer;
    bool _is_found;
    TribeModel *_model;
    CompleteModel *_complete_model;
public:
    explicit CurveFilter(TribeModel *model, QWidget *parent = nullptr);

    inline void setCompleteModel(CompleteModel *model)
    {
        _complete_model = model;
        _completer.setModel(_complete_model);
    }

    Qt::CaseSensitivity caseSensitivity() const;
    void setCaseSensitivity(Qt::CaseSensitivity cs);

    QRegExp::PatternSyntax patternSyntax() const;
    void setPatternSyntax(QRegExp::PatternSyntax s);

    Tribe::Selection selection() const;
    void setSelection(Tribe::Selection selection);

public slots:
    void setFound(bool is_found);
private:
    void setup();
protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void changeCaseSensitive();

    void changeSelection(QAction *action);

    void changePatternSyntax(QAction *action);

signals:
    void filterChanged();

};


#endif //REFINE_CURVEFILTER_HPP
