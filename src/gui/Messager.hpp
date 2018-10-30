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

class Messager : public QTextEdit
{
Q_OBJECT
public:
    enum MessageType
    {
        Info = 0x01,
        Warning = 0x02,
        Critical = 0x04,
        Fatal = 0x08,
        Debug = 0x10
    };
    static const int DURATION[4];
    class Cell
    {
    private:
        QDateTime _time;
        MessageType _type;
        QString _text;
    public:
        Cell() : _time(QDateTime::currentDateTime()),
                 _type(Info),
                 _text() {}

        Cell(MessageType type, const QString &text) :
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
protected:
    void contextMenuEvent(QContextMenuEvent *e) override;
private:
    QAction *_menu_all;
    QAction *_menu_clear;
public:
    explicit Messager(QWidget *parent = nullptr);

    static QString typeStr(MessageType type);

    inline void setShowTypes(int types) { _show_types = types; }

public slots:
    void showMessage(int type, const QString &msg);

private:
    void setup();
};


#endif //REFINE_MESSAGER_HPP
