//
// Created by jiang.wenqiang on 2018/10/23.
//

#include "Display.hpp"
#include "CurveViewer.hpp"
#include "Sketch.hpp"

Display::Display(QWidget *parent, Revolve *revolve, Message *message) :
        QSplitter(parent),
        _viewer(new CurveViewer(this, revolve, this)),
        Message(message),
        _presentation(false)
{
    setup();
}

void Display::setup()
{
    _left_panel = new QWidget(this);
    _toolbar = new QToolBar(_left_panel);
    setOrientation(Qt::Horizontal);
    addWidget(_left_panel);
    _layout = new QVBoxLayout(_left_panel);
    _layout->setMargin(0);
    _left_panel->setLayout(_layout);
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->addWidget(_toolbar);
    _layout->addWidget(_viewer);
    _zoom_plus = new QAction(QIcon(":res/icons/zoom+.png"), tr("放大"), _left_panel);
    _zoom_minus = new QAction(QIcon(":res/icons/zoom-.png"), tr("缩小"), _left_panel);
    _toolbar->addAction(_zoom_plus);
    _toolbar->addAction(_zoom_minus);

    _right_tab = new QTabWidget(this);
    _right_tab->setTabPosition(QTabWidget::South);
    addWidget(_right_tab);
    setStretchFactor(0 , 45);
    setStretchFactor(1 , 17);
    _right_tab->setVisible(false);
    _before_fullscreen = windowFlags();
}

Sketch &Display::sketch()
{
    return _viewer->sketch();
}

void Display::enablePresentation()
{
    hide();
    _right_tab->clear();
    _parent_layout->removeWidget(this);
    for (auto &iter : _docks) {
//        qDebug() << "Display::enablePresentation " << (iter->widget() == nullptr);
        _right_tab->addTab(iter->widget(), iter->windowTitle());
    }
    _right_tab->setVisible(true);
    setWindowFlag(Qt::Window);
    showFullScreen();
    _presentation = true;
}

void Display::disablePresentation()
{
    setWindowFlags(_before_fullscreen);
    hide();
    setWindowFlag(Qt::Widget);
    for (int i = 0; i < _docks.size(); ++i) {
        if (_widgets[i]) {
            _docks[i]->setWidget(_widgets[i]);
        }
    }
    _right_tab->setVisible(false);
    _parent_layout->addWidget(this);
    show();
    _presentation = false;
    emit exitPresentation();
}

void Display::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_F12:
            disablePresentation();
            break;
        default:
            break;
    }
}


void Display::closeEvent(QCloseEvent *event)
{
    if (_presentation) {
        disablePresentation();
        event->setAccepted(false);
    }
}
