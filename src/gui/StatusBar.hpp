//
// Created by jiang.wenqiang on 2018/10/11.
//

#ifndef REFINE_STATUSBAR_HPP
#define REFINE_STATUSBAR_HPP

#include <QtWidgets/QStatusBar>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include "Log.hpp"

/*!
 * @brief 状态栏
 */

class Logger;

class StatusLabel : public QLabel
{
Q_OBJECT
private:
    QString _prefix;
public:
    StatusLabel(QWidget *parent, const QString &prefix);

public slots:

    inline void setTextWithPrefix(const QString &string)
    {
        QLabel::setText(_prefix + string);
    };
};

class StatusBar : public QStatusBar, public Logger
{
Q_OBJECT
private:
    QVBoxLayout *_layout;
    StatusLabel *_label_info;
    StatusLabel *_label_warning;
    StatusLabel *_label_can;
    StatusLabel *_label_curve;
public:
    explicit StatusBar(QWidget *parent = nullptr);

private:
    void setupUi();

public:
    void displayMessage(QtMsgType type, const QString &info);

};


#endif //REFINE_STATUSBAR_HPP
