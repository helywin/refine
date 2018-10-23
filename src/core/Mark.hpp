//
// Created by jiang.wenqiang on 2018/10/23.
//

#ifndef REFINE_MARK_HPP
#define REFINE_MARK_HPP

#include <QtCore/QRect>
#include <QtCore/QLine>
#include <QtCore/QVector>

class MarkCell {
public:
    virtual QString str() const = 0;
};

class Mark
{
public:
    class RectCell : public MarkCell
    {
    private:
        QRect _data;
        QString _text;
    };

    class ArrowCell : public MarkCell
    {
    private:
        QLine _data;
        QString _text;
    };
private:
//    QVector<RectCell> _rects;
//    QVector<ArrowCell> _arrows;

public:
    const QVector<RectCell> &rects() const;
    const QVector<ArrowCell> &arrows() const;

    void addRect(const QRect &rect, const QString &text);
    void addArrow(const QLine &arrow, const QString &text);

    QString str() const;
};


#endif //REFINE_MARK_HPP
