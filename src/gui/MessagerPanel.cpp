//
// Created by jiang.wenqiang on 2018/10/23.
//

#include <QtCore/QProcess>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtWidgets/QScrollBar>
#include <QtGui/QTextCursor>
#include <QtGui/QContextMenuEvent>
#include "MessagerPanel.hpp"
#include "Output.hpp"

MessagerPanel::MessagerPanel(QWidget *parent) :
        QTextEdit(parent)
{
    setup();
}

void MessagerPanel::setup()
{
    _show_types = 0xff;
    setFont(QFont("微软雅黑", 10));
    setLineWrapMode(QTextEdit::NoWrap);
    setReadOnly(true);
    setMinimumWidth(200);
    _info_format.setForeground(QBrush(Output::INFO));
    _warning_format.setForeground(QBrush(Output::WARNING));
    _critical_format.setForeground(QBrush(Output::CRITICAL));
    _fatal_format.setForeground(QBrush(Output::FATAL));
    _debug_format.setForeground(QBrush(Output::DEBUG));
//    setContextMenuPolicy(Qt::CustomContextMenu);
    _menu = new QMenu();
    _menu_copy = new QAction(tr("复制(&C)"), this);
    _menu_all = new QAction(tr("全选(&S)"), this);
    _menu_clear = new QAction(tr("清除(&C)"), this);
    _menu_open = new QAction(tr("文件(&S)"), this);
    _menu->addAction(_menu_copy);
    _menu->addAction(_menu_all);
    _menu->addAction(_menu_clear);
    _menu->addAction(_menu_open);
    connect(_menu_copy, &QAction::triggered,
            this, &MessagerPanel::copy, Qt::DirectConnection);
    connect(_menu_all, &QAction::triggered,
            this, &MessagerPanel::selectAll, Qt::DirectConnection);
    connect(_menu_clear, &QAction::triggered,
            this, &MessagerPanel::clear, Qt::DirectConnection);
    connect(_menu_open, &QAction::triggered,
            this, &MessagerPanel::openFile, Qt::DirectConnection);
}


QString MessagerPanel::Cell::str() const
{
    QString str = _time.toString("hh:mm:ss") + " - ";
    str += _text;
    return str;
}

QString MessagerPanel::typeStr(MessagerPanel::MessageType type)
{
    QString str;
    switch (type) {
        case Info:
            str = QObject::tr("信息");
            break;
        case Warning:
            str = QObject::tr("警告");
            break;
        case Critical:
            str = QObject::tr("严重");
            break;
        case Fatal:
            str = QObject::tr("致命");
            break;
        case Debug:
            str = QObject::tr("调试");
            break;
    }
    return str;
}

void MessagerPanel::showMessage(int type, const QString &msg)
{
    QTextCursor cursor = this->textCursor();
    cursor.clearSelection();
    if (!cursor.atEnd()) {
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        setTextCursor(cursor);
    }
    _messages.append(Cell((MessageType) type, msg));
    if ((unsigned int) type & (unsigned int) _show_types) {
        _logs.append(_messages.last().str());
        switch (type) {
            case Info:
                cursor.insertText(_logs.last() + "\n", _info_format);
                break;
            case Warning:
                cursor.insertText(_logs.last() + "\n", _warning_format);
                break;
            case Critical:
                cursor.insertText(_logs.last() + "\n", _critical_format);
                break;
            case Fatal:
                cursor.insertText(_logs.last() + "\n", _fatal_format);
                break;
            case Debug:
                cursor.insertText(_logs.last() + "\n", _debug_format);
                break;
            default:
                break;
        }
    }
    QScrollBar *scroll_v = verticalScrollBar();
    QScrollBar *scroll_h = horizontalScrollBar();
    if (scroll_v) {
        scroll_v->setSliderPosition(scroll_v->maximum());
    }
    if (scroll_h) {
        scroll_h->setSliderPosition(scroll_h->minimum());
    }
}

void MessagerPanel::contextMenuEvent(QContextMenuEvent *e)
{
    _menu->exec(QCursor::pos());
    e->accept();
}

void MessagerPanel::openFile()
{
    QTextCursor cursor = textCursor();
    QString file = cursor.selectedText();
    file = file.trimmed();
    file = file.replace(QChar('/'), QString("\\"));
    if (QFile::exists(file)) {
#ifdef Q_OS_WIN     //别的平台这代码就木有用
        qDebug() << file;
        QProcess::startDetached("explorer.exe /select, " + file);
#endif
    }
}




