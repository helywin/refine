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
    QStringList _prefixs = QStringList({"611"});
    bool _upper_cased = true;
    QList<QString> _sequence;
    int _index = -1;
    QFont _font = QFont("微软雅黑", 10);
public:
    explicit Command(QWidget *parent = nullptr);

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
