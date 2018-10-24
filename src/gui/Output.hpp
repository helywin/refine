//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_OUTPUT_HPP
#define REFINE_OUTPUT_HPP

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>

class Refine;
class Messager;
class Revolve;

class Output : public QDockWidget
{
Q_OBJECT
private:
    QWidget *_content;
    QHBoxLayout *_layout;
    Messager *_messager;
public:
    explicit Output(QWidget *parent);

private:
    void setup();

public:
    void connectToMessager(Refine *emitter);

    void connectToMessager(Revolve *emitter);
};


#endif //REFINE_OUTPUT_HPP
