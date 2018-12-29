//
// Created by jiang.wenqiang on 2018/12/27.
//

#ifndef REFINE_INFORMATION_HPP
#define REFINE_INFORMATION_HPP

#include <QtWidgets/QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>

class Refine;
class Information;
class Revolve;
class FilePicker;

class MessagePanel : public QWidget
{
Q_OBJECT
public:
    static const QColor INFO;
    static const QColor WARNING;
    static const QColor CRITICAL;
    static const QColor FATAL;
    static const QColor DEBUG;
private:
    QWidget *_content;
    QVBoxLayout *_layout;
    QWidget *_labels;
    QHBoxLayout *_layout_labels;
    QLabel *_label_info;
    QLabel *_label_warning;
    QLabel *_label_critical;
    QLabel *_label_fatal;
    QLabel *_label_debug;
    Information *_messager;
public:
    explicit MessagePanel(QWidget *parent = nullptr);

    void connectToMessager(Refine *emitter);

    void connectToMessager(Revolve *emitter);

    void connectToMessager(FilePicker *emitter);

private:
    void setup();

};


#endif //REFINE_INFORMATION_HPP
