//
// Created by jiang.wenqiang on 2018/9/28.
//

#include <QtWidgets/QApplication>
#include <QtCore/QDebug>
#include "RecvBuffer.hpp"
#include "Curve.hpp"
#include "Can.hpp"
#include "Revolve.hpp"
#include "Tribe.hpp"
#include "Collect.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle("fusion");
    Scope scope;
    scope.show();
    return QApplication::exec();
}
