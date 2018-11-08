//
// Created by jiang.wenqiang on 2018/11/7.
//

#include <QtCore/QDebug>
#include <QtWidgets/QLineEdit>
#include <QtGui/QPainter>
#include "SpecInput.hpp"

SpecInput::SpecInput(int len, QWidget *parent, int base) :
        QAbstractItemDelegate(parent),
        _len(len),
        _base(base) {}

QWidget *SpecInput::createEditor(QWidget *parent,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    auto *edit = new QLineEdit(parent);
    edit->setValidator(new QRegExpValidator(QRegExp()));
    if (_base == 16) {
        edit->setInputMask(QString("H").repeated(_len));
        edit->setToolTip(tr("输入16进制"));
    } else {
        edit->setInputMask(QString("H").repeated(_len));
        edit->setToolTip(tr("输入10进制"));
    }
    return edit;
}

void SpecInput::destroyEditor(QWidget *editor, const QModelIndex &index) const
{
    delete dynamic_cast<QLineEdit *>(editor);
}

void SpecInput::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    dynamic_cast<QLineEdit *>(editor)->setText(
            QString::number(index.data(Qt::EditRole).toInt(), _base)
    );
}

void SpecInput::setModelData(QWidget *editor, QAbstractItemModel *model,
                             const QModelIndex &index) const
{
    model->setData(index,
                   dynamic_cast<QLineEdit *>(editor)->text()
                           .toInt(nullptr, _base),
                   Qt::EditRole
    );
}

void SpecInput::updateEditorGeometry(QWidget *editor,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    dynamic_cast<QLineEdit *>(editor)->setFixedSize(option.rect.size());
    dynamic_cast<QLineEdit *>(editor)->move(option.rect.topLeft());
}

void SpecInput::paint(QPainter *painter, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    painter->drawText(option.rect.x() + 4,
                      option.rect.y() + (option.rect.height() / 2 + 5),
                      index.data().toString());
}

QSize SpecInput::sizeHint(const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    return option.rect.size();
}
