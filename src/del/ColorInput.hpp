//
// Created by jiang.wenqiang on 2018/11/8.
//

#ifndef REFINE_COLORINPUT_HPP
#define REFINE_COLORINPUT_HPP

#include <QtWidgets/QAbstractItemDelegate>

class ColorInput : public QAbstractItemDelegate
{
public:
    explicit ColorInput(QWidget *parent = nullptr);
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


#endif //REFINE_COLORINPUT_HPP
