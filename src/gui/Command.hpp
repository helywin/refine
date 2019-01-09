//
// Created by jiang.wenqiang on 2019/1/2.
//

#ifndef REFINE_COMMAND_HPP
#define REFINE_COMMAND_HPP

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>

class Command : public QLineEdit
{
Q_OBJECT
private:
    QStringList _prefixs = QStringList({"611"});
    QList<QString> _sequence;
    int _index = -1;
    QMargins _text_margins;
    QMenu *_menu;
    QAction *_menu_uppercased;
    QAction *_menu_clear;
    QToolButton *_settings;

public:
    explicit Command(QWidget *parent = nullptr);
    void clearCommand();
    void addCommandActions(QList<QAction *> actions);

public slots:
    void setPrefix(const QStringList &prefix);
private:
    void setup();
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private slots:
    void commandChange(const QString &string);

signals:
    void commandChanged(const QString &string);
    void commandConfirmed(const QString &string);
};


#endif //REFINE_COMMAND_HPP
