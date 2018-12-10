//
// Created by jiang.wenqiang on 2018/12/10.
//

#ifndef REFINE_SPININPUT_HPP
#define REFINE_SPININPUT_HPP


#include <QtWidgets/QSpinBox>
#include <QtWidgets/QAbstractItemDelegate>

class SpinInput : public QAbstractItemDelegate
{
Q_OBJECT
private:
    int _max;
    int _min;
    int _step;
    QString _prefix;
    QString _suffix;

public:
    explicit SpinInput(int max, int min, QWidget *parent = nullptr, int step = 1,
                       const QString &prefix = QString(), const QString &suffix = QString());
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void destroyEditor(QWidget *editor,
                       const QModelIndex &index) const override;
    void
    setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void
    updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};

#endif //REFINE_SPININPUT_HPP
