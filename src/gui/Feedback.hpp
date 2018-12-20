//
// Created by jiang.wenqiang on 2018/12/20.
//

#ifndef REFINE_FEEDBACK_HPP
#define REFINE_FEEDBACK_HPP

#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>

class Feedback : public QDialog
{
Q_OBJECT
private:
    QVBoxLayout *_layout;
    QLineEdit *_title;
    QTextEdit *_text;
    QPushButton *_send;

public:
    explicit Feedback(QWidget *parent = nullptr);

private slots:
    void sendFeedback();
private:
    void setup();
};


#endif //REFINE_FEEDBACK_HPP
