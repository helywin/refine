//
// Created by jiang.wenqiang on 2018/11/14.
//

#ifndef REFINE_FINITEPROGRESS_HPP
#define REFINE_FINITEPROGRESS_HPP

#include <QtWidgets/QWidget>

class FiniteProgress : public QWidget
{
Q_OBJECT
private:
    int _value;
    int _maximum;
    int _minimum;
    int _border_width;
    QColor _border_color;
    QColor _content_color;
public:
    explicit FiniteProgress(QWidget *parent = nullptr);

    inline void setValue(int value)
    {
        _value = value;
        update();
    }

    inline int value() const { return _value; }

    inline void setMaximum(int maximun)
    {
        _maximum = maximun;
        update();
    }

    inline int maximum() const { return _maximum; }

    inline void setMinimum(int minimun)
    {
        _minimum = minimun;
        update();
    }

    inline int minimum() const { return _minimum; }

    inline void setBorderWidth(int width)
    {
        _border_width = width;
        update();
    }

    inline int borderWidth() const { return _border_width; }

    inline void setBorderColor(const QColor &color)
    {
        _border_color = color;
        update();
    }

    inline QColor borderColor() const { return _border_color; }

    inline void setContentColor(const QColor &color)
    {
        _content_color = color;
        update();
    }

    inline QColor contentColor() const { return _content_color; }

protected:
    void paintEvent(QPaintEvent *event) override;
};


#endif //REFINE_FINITEPROGRESS_HPP
