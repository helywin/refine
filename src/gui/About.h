//
// Created by jiang.wenqiang on 2018/7/20.
//

#ifndef REFINE_ABOUT_H
#define REFINE_ABOUT_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextBrowser>

class About : public QDialog {
Q_OBJECT
private:
    QLabel *logo;
    QLabel *name;
    QLabel *version;
    QLabel *intro;
    QLabel *copyright;
    QLabel *soft;
    QTextBrowser *license;

public:
    explicit About(QWidget *parent = nullptr);

    ~About() final = default;

protected:
    void setup_ui();

    bool eventFilter(QObject *object, QEvent *event) override;

};


#endif //REFINE_ABOUT_H
