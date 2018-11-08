//
// Created by jiang.wenqiang on 2018/11/8.
//

#include <QtCore/QString>
#include <QtGui/QPainter>
#include "ColorInput.hpp"

//
// Created by jiang.wenqiang on 2018/11/7.
//

#include <QtCore/QDebug>
#include <QtWidgets/QLineEdit>
#include <QtGui/QPainter>
#include "ColorInput.hpp"

ColorInput::ColorInput(QWidget *parent) :
        QAbstractItemDelegate(parent) {}

QWidget *ColorInput::createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{
    auto *edit = new QLineEdit(parent);
    edit->setValidator(new QRegExpValidator(QRegExp("[0-9a-fA-F]{0,6}")));
//    edit->setInputMask(QString("H").repeated(6));
    edit->setToolTip(tr("颜色码"));
    return edit;
}

void ColorInput::destroyEditor(QWidget *editor, const QModelIndex &index) const
{
    delete dynamic_cast<QLineEdit *>(editor);
}

void ColorInput::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    dynamic_cast<QLineEdit *>(editor)->setText(
            QString::number(index.data(Qt::EditRole).toInt(), 16)
    );
}

void ColorInput::setModelData(QWidget *editor, QAbstractItemModel *model,
                              const QModelIndex &index) const
{
    model->setData(index,
                   dynamic_cast<QLineEdit *>(editor)->text()
                           .toInt(nullptr, 16),
                   Qt::EditRole
    );
}

void ColorInput::updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    dynamic_cast<QLineEdit *>(editor)->setFixedSize(option.rect.width() - 44,
                                                    option.rect.height());
    dynamic_cast<QLineEdit *>(editor)->move(option.rect.left() + 44,
                                            option.rect.top());
}

void ColorInput::paint(QPainter *painter, const QStyleOptionViewItem &option,
                       const QModelIndex &index) const
{
    painter->setBrush(QBrush(QColor(index.data(Qt::EditRole).toUInt())));
    painter->drawRect(option.rect.x() + 2, option.rect.y() + 2,
                      40, option.rect.height() - 4);
    painter->drawText(option.rect.x() + 44,
                      option.rect.y() + (option.rect.height() / 2 + 5),
                      index.data().toString());
}

QSize ColorInput::sizeHint(const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    return option.rect.size();
}
