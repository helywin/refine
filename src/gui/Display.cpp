//
// Created by jiang.wenqiang on 2018/10/23.
//

#include <QtGui/QKeyEvent>
#include <QtPlatformHeaders/QWindowsWindowFunctions>
#include "Display.hpp"

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
    _menubar = new QMenuBar(_left_panel);
    _menu_zoom = new QMenu(tr("放大"), _menubar);
    _menu_zoom_axis = new QActionGroup(_menu_zoom);
    _menu_zoom_x = new QAction(tr("x轴"), _menu_zoom_axis);
    _menu_zoom_x->setCheckable(true);
    _menu_zoom_x->setChecked(true);
    _menu_zoom_y = new QAction(tr("y轴"), _menu_zoom_axis);
    _menu_zoom_y->setCheckable(true);
    _menu_zoom_xy = new QAction(tr("x+y轴"), _menu_zoom_axis);
    _menu_zoom_xy->setCheckable(true);
    _menu_zoom_axis->setExclusive(true);
    _menu_zoom_group = new QActionGroup(_menu_zoom);
    _menu_zoom_plus = new QAction(tr("放大"), _menu_zoom_group);
    _menu_zoom_plus->setShortcut(QKeySequence("="));
    _menu_zoom_plus->setCheckable(true);
    _menu_zoom_plus->setChecked(true);
    _menu_zoom_minus = new QAction(tr("缩小"), _menu_zoom_group);
    _menu_zoom_minus->setShortcut(QKeySequence("-"));
    _menu_zoom_minus->setCheckable(true);
    _menu_zoom_group->setExclusive(true);
    _menu_zoom_origin = new QAction(tr("默认"), _menu_zoom);
    _menu_zoom_origin->setShortcut(QKeySequence("D"));
    _menu_zoom_minimum = new QAction(tr("最小"), _menu_zoom);
    _menu_zoom_minimum->setShortcut(QKeySequence("M"));
    _menu_zoom->addActions(_menu_zoom_axis->actions());
    _menu_zoom->addSeparator();
    _menu_zoom->addActions(_menu_zoom_group->actions());
    _menu_zoom->addSeparator();
    _menu_zoom->addAction(_menu_zoom_origin);
    _menu_zoom->addAction(_menu_zoom_minimum);
    _btn_zoom = new QPushButton(QIcon(":res/icons/zoom+.png"), tr(""), _left_panel);
    _btn_zoom->setIconSize(QSize(32, 32));
//    _btn_zoom->setMenu(_menu_zoom);
    _btn_zoom->setContentsMargins(0, 0, 0, 0);
    _toolbar->addWidget(_btn_zoom);
    _menubar->addMenu(_menu_zoom);
//    _menubar->addActions(_menu_zoom->actions());
    _layout->setMenuBar(_menubar);
    _right_tab = new QTabWidget(this);
    _right_tab->setTabPosition(QTabWidget::South);
    addWidget(_right_tab);
    setStretchFactor(0, 45);
    setStretchFactor(1, 17);
    _right_tab->setVisible(false);
    _before_fullscreen = windowFlags();
    connect(_menu_zoom_axis, &QActionGroup::triggered, this,
            [=](QAction *action) {
                if (action == _menu_zoom_x) {
                    _viewer->setXZoom(true);
                    _viewer->setYZoom(false);
                } else if(action == _menu_zoom_y) {
                    _viewer->setYZoom(true);
                    _viewer->setXZoom(false);
                } else {
                    _viewer->setXZoom(true);
                    _viewer->setYZoom(true);
                }
            });
    connect(_menu_zoom_plus, &QAction::triggered, _viewer, &CurveViewer::zoomPlus);
    connect(_menu_zoom_minus, &QAction::triggered, _viewer, &CurveViewer::zoomMinus);
    connect(_menu_zoom_origin, &QAction::triggered, _viewer, &CurveViewer::zoomDefault);
    connect(_menu_zoom_minimum, &QAction::triggered, _viewer, &CurveViewer::zoomMinimum);
    connect(_menu_zoom_group, &QActionGroup::triggered,
            this, &Display::changeZoomMode, Qt::DirectConnection);
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
    QWindowsWindowFunctions::
    setHasBorderInFullScreen(this->windowHandle(), true);   //解决全屏无菜单的问题
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

void Display::changeZoomMode(QAction *action)
{
    _menu_zoom->setTitle(action->text());
    if (action == _menu_zoom_plus) {
        _btn_zoom->setIcon(QIcon(":res/icons/zoom+.png"));
    } else {
        _btn_zoom->setIcon(QIcon(":res/icons/zoom-.png"));
    }
}
