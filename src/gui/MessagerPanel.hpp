//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_MESSAGER_HPP
#define REFINE_MESSAGER_HPP

#include <QtCore/QVector>
#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include <QtWidgets/QDialog>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include "Global.hpp"

class MessagerPanel : public QTextEdit
{
Q_OBJECT
public:

    class Cell
    {
    private:
        QDateTime _time;
        Re::MessageType _type;
        QString _text;
    public:
        Cell() : _time(QDateTime::currentDateTime()),
                 _type(Re::Info),
                 _text() {}

        Cell(Re::MessageType type, const QString &text) :
                _time(QDateTime::currentDateTime()),
                _type(type),
                _text(qMove(text)) {}

        QString str() const;

        inline QString type() const { return typeStr(_type); }

        inline const QString &text() const { return _text; }
    };

private:
    //ui

    //data
    QVector<Cell> _messages;
    QStringList _logs;
    int _show_types;
    QTextCharFormat _info_format;
    QTextCharFormat _warning_format;
    QTextCharFormat _critical_format;
    QTextCharFormat _fatal_format;
    QTextCharFormat _debug_format;
    QMenu *_menu;
    QAction *_menu_copy;
    QAction *_menu_all;
    QAction *_menu_clear;
    QAction *_menu_open;
public:
    explicit MessagerPanel(QWidget *parent = nullptr);

    static QString typeStr(Re::MessageType type);

    inline void setShowTypes(int types) { _show_types = types; }

public slots:
    void showMessage(int type, const QString &msg);
protected:
    void contextMenuEvent(QContextMenuEvent *e) override;
private:
    void setup();
private slots:
    void openFile();
};


#endif //REFINE_MESSAGER_HPP
