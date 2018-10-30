//
// Created by jiang.wenqiang on 2018/10/30.
//

#ifndef REFINE_CHANGELOG_HPP
#define REFINE_CHANGELOG_HPP

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTextEdit>
#include "LostFocus.hpp"

class ChangeLog : public LostFocus
{
Q_OBJECT
private:
    QVBoxLayout *_layout;
    QTextEdit *_text;
public:
    explicit ChangeLog(QWidget *parent = nullptr);
private:
    void setup();
};


#endif //REFINE_CHANGELOG_HPP
