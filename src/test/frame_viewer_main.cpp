//
// Created by jiang.wenqiang on 2018/9/20.
//

#include <QtWidgets/QApplication>
#include "FrameViewer.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle("fusion");
    FrameViewer frame_viewer;
    frame_viewer.show();
    frame_viewer.activateWindow();
    return QApplication::exec();
}