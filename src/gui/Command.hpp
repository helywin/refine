//
// Created by jiang.wenqiang on 2019/1/2.
//

#ifndef REFINE_COMMAND_HPP
#define REFINE_COMMAND_HPP

#include <QtWidgets/QLineEdit>

class Command : public QLineEdit
{
Q_OBJECT
private:
    bool _upper_cased = true;
    bool _clear_flag = true;
    QList<QString> _sequence;
    int _index = -1;
public:
    explicit Command(QWidget *parent = nullptr);

private:
    void setup();
protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void commandChange(const QString &string);

signals:
    void commandChanged(const QString &string);
    void commandConfirmed(const QString &string);
};


#endif //REFINE_COMMAND_HPP
