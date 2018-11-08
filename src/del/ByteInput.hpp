//
// Created by jiang.wenqiang on 2018/11/7.
//

#ifndef REFINE_BYTEINPUT_HPP
#define REFINE_BYTEINPUT_HPP

#include <QtWidgets/QAbstractItemDelegate>
#include "ByteRangeFrame.hpp"

class ByteInput : public QAbstractItemDelegate
{
Q_OBJECT
private:
    bool _checkable;
public:
    explicit ByteInput(bool checkable, QWidget *parent = nullptr);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
    void destroyEditor(QWidget *editor,
                       const QModelIndex &index) const override;
    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;
};


#endif //REFINE_BYTEINPUT_HPP
