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

class TribeModel;

class CurveFilter : public QLineEdit
{
Q_OBJECT
public:
    enum Selection
    {
        SelectChecked,
        SelectUnchecked,
        SelectAll
    };
private:
    QMenu *_menu;
    QAction *_case_sensitive;
    QActionGroup *_curve_items;
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
public:
    explicit CurveFilter(TribeModel *model, QWidget *parent = nullptr);

public slots:
    void setFound(bool is_found);
private:
    void setup();
protected:
    void paintEvent(QPaintEvent *event) override;

};


#endif //REFINE_CURVEFILTER_HPP
