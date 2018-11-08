//
// Created by jiang.wenqiang on 2018/11/7.
//

#ifndef REFINE_SPECINPUT_HPP
#define REFINE_SPECINPUT_HPP

#include <QtWidgets/QSpinBox>
#include <QtWidgets/QAbstractItemDelegate>

class SpecInput : public QAbstractItemDelegate
{
Q_OBJECT
private:
    int _len;
    int _base;
public:
    explicit SpecInput(int len, QWidget *parent = nullptr, int base = 10);
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


#endif //REFINE_SPECINPUT_HPP
