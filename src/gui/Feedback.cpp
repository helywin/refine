//
// Created by jiang.wenqiang on 2018/12/20.
//

#include "Feedback.hpp"
#include <QtGui/QDesktopServices>
#include <QtCore/QProcessEnvironment>

Feedback::Feedback(QWidget *parent) :
        QDialog(parent)
{
    setup();
}

void Feedback::setup()
{
    QFont font("微软雅黑", 10);
    setWindowTitle(tr("反馈"));
    _layout = new QVBoxLayout(this);
    setLayout(_layout);
    _title = new QLineEdit(this);
//    QString email_address = QProcessEnvironment::
//                            systemEnvironment().value("USERNAME") + QString("@byd.com");
    _title->setFont(font);
    _title->setText(tr("问题反馈"));
    _text = new QTextEdit(this);
    _text->setFont(font);
    _text->setText(tr("示例内容"));
    _send = new QPushButton(this);
    _send->setIcon(QIcon(":res/ui/logo_48x48.png"));
    _send->setContentsMargins(0, 0, 0, 0);
    _send->setFixedSize(48, 48);
    _layout->addWidget(_title);
    _layout->addWidget(_text);
    _layout->addWidget(_send);
    connect(_send, &QPushButton::clicked,
            this, &Feedback::sendFeedback, Qt::DirectConnection);
}

void Feedback::sendFeedback()
{
    QString email_address("收件人: jiang.wenqiang@byd.com");
    QString url = QString("mailto:%1?subject=%2&body=%3").
            arg(email_address).
            arg(_title->text()).
            arg(_text->toPlainText());
    QDesktopServices::openUrl(QUrl(url));
}
