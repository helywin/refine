//
// Created by jiang.wenqiang on 2019/1/2.
//

#ifndef REFINE_CANMESSAGE_HPP
#define REFINE_CANMESSAGE_HPP

#include <QtWidgets/QPlainTextEdit>

class CanMessage : public QPlainTextEdit
{
Q_OBJECT
public:
    enum MessageType
    {
        Send = 0x01,
        Ascii = 0x02,
        Hex = 0x04,
    };

private:
    QTextCharFormat _send_format;
    QTextCharFormat _ascii_format;
    QTextCharFormat _hex_format;
public:
    explicit CanMessage(QWidget *parent = nullptr);

public slots:
    void insertMessage(const QString &msg, MessageType type);

private:
    void setup();
protected:
    void paintEvent(QPaintEvent *e) override;
};


#endif //REFINE_CANMESSAGE_HPP
