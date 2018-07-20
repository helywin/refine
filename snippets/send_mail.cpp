//
// Created by jiang.wenqiang on 2018/7/20.
//

#include <QtCore/QString>
#include <QtGui/QDesktopServices>

int main() {
    QString url = QString("mailto:%1?subject%2&body%3").arg().arg().arg();
    QDesktopServices::setUrlHandler();
    QDesktopServices::openUrl(url);
}