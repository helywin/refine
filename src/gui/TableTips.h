//
// Created by jiang.wenqiang on 2018/7/25.
//

#ifndef REFINE_TABLETIPS_H
#define REFINE_TABLETIPS_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QRadioButton>
#include <QtCore/QEvent>
#include <QtWidgets/QSpinBox>

class TableTips : public QDialog {
Q_OBJECT

    friend class GetVal;

protected:
    QVBoxLayout *layout;
    QVBoxLayout *button_layout;
    QWidget *main_widget;
    QVBoxLayout *main_layout;
    QWidget *button_widget;
    QPushButton *ok_button;
public:
    TableTips() = delete;

    explicit TableTips(QWidget *parent = nullptr);

    virtual void regen(QPoint pos) = 0;

protected:
    virtual void setupUi();

protected slots:

    virtual void setVal() = 0;

signals:

    virtual void val(QString s);

protected:
    bool eventFilter(QObject *object, QEvent *event) override;
};


class TipType : public TableTips {
Q_OBJECT
protected:
    QVBoxLayout *radio_layout;
    QGroupBox *type;
    QRadioButton *physical;
    QRadioButton *logical;
public:
    explicit TipType(QWidget *parent = nullptr);

    void regen(QPoint pos) override;

private:
    void setupUi() override;

protected slots:

    void setVal() override;

};

class TipZero : public TableTips {
Q_OBJECT
protected:
    QVBoxLayout *group_layout;
    QGroupBox *zero;
    QSpinBox *line;
public:
    explicit TipZero(QWidget *parent = nullptr);

    void regen(QPoint pos) override;

private:
    void setupUi() override;

protected:
    void setVal() override;
};

class TipSample : public TableTips {
Q_OBJECT
protected:
    QVBoxLayout *group_layout;
    QRadioButton *time;
    QRadioButton *frame;
    QGroupBox *sample;
    QSpinBox *line;
public:
    explicit TipSample(QWidget *parent = nullptr);

    void regen(QPoint pos) override;

private:
    void setupUi() override;

protected:
    void setVal() override;

private slots:

    void changeSelection();
};

class GetVal : public QObject {
Q_OBJECT
public:
    enum class Type {
        Type,
        Width,
        Color,
        ZeroByte,
        HighByte,
        LowByte,
        Sample,
        RangeIn,
        RangeOut
    };
private:
    int row;
    int column;
    TipType *type;
    TipZero *zero;
    TipSample *sample;

public:
    GetVal() = delete;

    GetVal(const GetVal &v) = delete;

    explicit GetVal(QWidget *parent = nullptr);

    void get(GetVal::Type t, int row, int column, QPoint pos);

    GetVal &operator=(const GetVal &v) = delete;

signals:

    void getVal(QString s, int row, int column);

public slots:

    void gotVal(QString s);

};


#endif //REFINE_TABLETIPS_H
