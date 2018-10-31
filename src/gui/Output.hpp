//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_OUTPUT_HPP
#define REFINE_OUTPUT_HPP

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>

class Refine;
class Messager;
class Revolve;
class FilePicker;

class Output : public QDockWidget
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
    Messager *_messager;
    QAction *_visible;
public:
    explicit Output(QWidget *parent);
protected:
    void closeEvent(QCloseEvent *event) override;
public:
    inline void setAction(QAction *action) { _visible = action; }

private:
    void setup();

public:
    void connectToMessager(Refine *emitter);

    void connectToMessager(Revolve *emitter);

    void connectToMessager(FilePicker *emitter);
};


#endif //REFINE_OUTPUT_HPP
