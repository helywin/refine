//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_MARK_HPP
#define REFINE_MARK_HPP

#include <QtGui/QColor>
#include <QtCore/QRect>
#include <QtCore/QLine>
#include <QtCore/QVector>
#include <QtCore/QVariant>
#include "Message.hpp"

class Mark : public Message
{
public:
    enum MarkType
    {
        Rect,
        Circle,
        Arrow,
        Line,
        Distance,
        Text,
    };

    class Cell
    {
    private:
        QString _name;
        QVariant _data;
        QString _curve;     //!> 所属的曲线，可为空
        QString _text;
        QColor _color;
        int size;       //字体大小或者线条宽度
    public:
        QString str() const;
    };
private:
    QVector<Cell> _marks;

public:
    const QVector<Cell> &marks() const;

    void addMark(const QVariant &rect, MarkType type, const QString &text);

    QString str() const;
};


#endif //REFINE_MARK_HPP
