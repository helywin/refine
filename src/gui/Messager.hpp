//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_MESSAGE_HPP
#define REFINE_MESSAGE_HPP

#include <QtWidgets/QDialog>

class Messager : public QDialog
{
Q_OBJECT
public:
    enum MessageType
    {
        Info = 0,
        Warning = 1,
        Critical = 2,
        Fatal = 3
    };

    static const int SEC[4];
private:
public:
    explicit Messager(QWidget *parent);

    void info(QString &&msg);
    void warning(QString &&msg);
    void critical(QString &&msg);
    void fatal(QString &&msg);

private:
    void setup();
};


#endif //REFINE_MESSAGE_HPP
