//
// Created by jiang.wenqiang on 2018/9/20.
//

#include <QtCore/QDebug>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QHeaderView>
#include "FrameViewer.hpp"
#include "Buffer.hpp"
#include "File.hpp"

FrameViewer::FrameViewer(QWidget *parent) : QMainWindow(parent)
{
    setupUi();
}

void FrameViewer::setupUi()
{
    this->setWindowTitle("报文查看器");
    this->resize(860, 600);
    this->move(200, 50);
    _menubar = new QMenuBar(this);
    this->setMenuBar(_menubar);
    _menu_file = new QMenu(QString("文件(&F)"), _menubar);
    _menu_file_open = new QAction(QString("打开(&O)"), _menu_file);
    _menu_file_exit = new QAction(QString("退出(&E)"), _menu_file);
    _menu_file->addAction(_menu_file_open);
    _menu_file->addAction(_menu_file_exit);
    _menu_help = new QMenu(QString("帮助(&H)"), _menubar);
    _menu_help_about = new QAction(QString("关于(&A)"), _menu_help);
    _menu_help->addAction(_menu_help_about);
    _menubar->addMenu(_menu_file);
    _menubar->addMenu(_menu_help);
    _menubar->addAction(_menu_file->menuAction());
    _menubar->addAction(_menu_help->menuAction());

    _widget_central = new QWidget(this);
    this->setCentralWidget(_widget_central);
    _layout_central = new QHBoxLayout(_widget_central);
    _widget_central->setLayout(_layout_central);

    _table_central = new QTableWidget(_widget_central);
    _layout_central->addWidget(_table_central);

    _file_dialog = new QFileDialog(this);
    _file_dialog->setWindowTitle(QString("打开报文文件"));
    connect(_menu_file_open, &QAction::triggered, _file_dialog,
            &QFileDialog::show);
    connect(_file_dialog, &QFileDialog::fileSelected, this,
            &FrameViewer::readFrameData);
    QString fname("D:/jiang.wenqiang/code/refine/data/data.fmd");
    QFile f(fname);
    if (f.exists()) {
        readFrameData(fname);
    }
}

void FrameViewer::readFrameData(const QString &fname)
{
    QFile f(fname);
    Buffer buffer;
    File file;
    int size;
    int frame_num;
    int index = 0;
    if (!file.loadFrameRecordBegin(f, buffer, &size, &frame_num)) {
        qDebug("file error!");
        return;
    }
    QFont consolas("Consolas");
    consolas.setPointSize(12);
    _table_central->clear();
    _table_central->setColumnCount(11);
    if (frame_num > 100000) {
        frame_num = 100000;
        QMessageBox::warning(this, QString("警告"), QString("数据超过10w条，只显示前10w条！"));
    }
    _table_central->setRowCount(frame_num);
    _table_central->setHorizontalHeaderLabels(
            QStringList({"序号", "帧包", "ID", "时间", "标识", "类型",
                         "远程", "扩展", "长度", "报文", "保留",}));
    _table_central->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _table_central->setColumnWidth(0, 60);
    _table_central->setColumnWidth(1, 60);
    _table_central->setColumnWidth(2, 60);
    _table_central->setColumnWidth(3, 80);
    _table_central->setColumnWidth(4, 40);
    _table_central->setColumnWidth(5, 60);
    _table_central->setColumnWidth(6, 40);
    _table_central->setColumnWidth(7, 40);
    _table_central->setColumnWidth(8, 40);
    _table_central->setColumnWidth(9, 240);
    _table_central->setColumnWidth(10, 100);
    _table_central->setSelectionMode(
            QAbstractItemView::SelectionMode::SingleSelection);
    _table_central->setSelectionBehavior(
            QAbstractItemView::SelectionBehavior::SelectRows);
    _table_central->verticalHeader()->hide();
    for (int i = 0; i < size; ++i) {
        if (index > frame_num) {
            break;
        }
        QString str;
        file.loadFrameRecord(buffer);
        const Buffer::Cell &cell = buffer.tail();
        for (int j = 0; j < cell.dataSize(); ++j) {
            auto item = new QTableWidgetItem(QString::number(index));
            item->setFont(consolas);
            _table_central->setItem(index, 0, item);
            item = new QTableWidgetItem(QString::number(i));
            item->setFont(consolas);
            _table_central->setItem(index, 1, item);
            item = new QTableWidgetItem(
                    QString("0x") +
                    QString::number(cell[j]->ID, 16).toUpper());
            item->setFont(consolas);
            _table_central->setItem(index, 2, item);
            item = new QTableWidgetItem(
                    QString::number(cell[j]->TimeStamp));
            item->setFont(consolas);
            _table_central->setItem(index, 3, item);
            if (cell[j]->TimeFlag) {
                str = QString("√");
            } else {
                str = QString("×");
            }
            _table_central->setItem(index, 4, new QTableWidgetItem(str));
            switch (cell[j]->SendType) {
                case 0:
                    str = QString("正常");
                    break;
                case 1:
                    str = QString("单次");
                    break;
                case 2:
                    str = QString("自收发");
                    break;
                case 3:
                    str = QString("单次自收发");
                    break;
                default:
                    str = QString("?");
            }
            _table_central->setItem(index, 5, new QTableWidgetItem(str));
            if (cell[j]->RemoteFlag) {
                str = QString("√");
            } else {
                str = QString("×");
            }
            _table_central->setItem(index, 6, new QTableWidgetItem(str));
            if (cell[j]->ExternFlag) {
                str = QString("√");
            } else {
                str = QString("×");
            }
            _table_central->setItem(index, 7, new QTableWidgetItem(str));
            _table_central->
                    setItem(index, 8, new QTableWidgetItem(
                    QString::number(cell[j]->DataLen)));
            str.clear();
            for (const auto &data : cell[j]->Data) {
                str += QString(" %1").arg(data, 2, 16, QChar('0')).toUpper();
            }
            item = new QTableWidgetItem(str);
            item->setFont(consolas);
            _table_central->setItem(index, 9, item);
            str.clear();
            for (const auto &data : cell[j]->Reserved) {
                str += QString(" %1").arg(data, 2, 16, QChar('0')).toUpper();
            }
            item = new QTableWidgetItem(str);
            item->setFont(consolas);
            _table_central->setItem(index, 10, item);
            index += 1;
        }
        buffer.tailForward();
    }
}
