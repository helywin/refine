//
// Created by jiang.wenqiang on 2018/10/24.
//

#ifndef REFINE_MARKBOX_HPP
#define REFINE_MARKBOX_HPP

#include <QtWidgets/QDockWidget>

class MarkBox : public QDockWidget
{
    Q_OBJECT
private:
    QAction *_visible;
public:
    explicit MarkBox(QWidget *parent);

    inline void setAction(QAction *action) { _visible = action; }
private:
    void setup();
protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif //REFINE_MARKBOX_HPP
