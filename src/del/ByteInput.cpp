//
// Created by jiang.wenqiang on 2018/11/7.
//

#include <QtGui/QPainter>
#include <QtCore/QDebug>
#include "ByteInput.hpp"

ByteInput::ByteInput(bool checkable, QWidget *parent) :
        _checkable(checkable),
        QAbstractItemDelegate(parent) {}

QWidget *ByteInput::createEditor(QWidget *parent,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    return new ByteRangeFrame(_checkable, parent);
}

void ByteInput::destroyEditor(QWidget *editor, const QModelIndex &index) const
{
    delete dynamic_cast<ByteRangeFrame *>(editor);
}

void ByteInput::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    dynamic_cast<ByteRangeFrame *>(editor)
            ->setData(index.data(Qt::EditRole).toString());
}

void ByteInput::setModelData(QWidget *editor, QAbstractItemModel *model,
                             const QModelIndex &index) const
{
    qDebug() << dynamic_cast<ByteRangeFrame *>(editor)->data();
    model->setData(index,
                   dynamic_cast<ByteRangeFrame *>(editor)->data(),
                   Qt::EditRole
    );
}

void ByteInput::updateEditorGeometry(QWidget *editor,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    int w;
    if (dynamic_cast<ByteRangeFrame *>(editor)->checkable()) {
        w = 170;
    } else {
        w = 140;
    }
    dynamic_cast<ByteRangeFrame *>(editor)->setFixedSize(w, 29);
    dynamic_cast<ByteRangeFrame *>(editor)->move(option.rect.topLeft());
}

void ByteInput::paint(QPainter *painter, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    painter->drawText(option.rect.x() + 4,
                      option.rect.y() + (option.rect.height() / 2 + 5),
                      index.data().toString());
}

QSize ByteInput::sizeHint(const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    return QSize();
}
