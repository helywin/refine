/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/11
 * @brief 软件信息
 * @details 软件信息存储
 ******************************************************************************/

#include "Version.hpp"

/*!
 * @brief 返回版本字符串
 * @return 版本字符串
 */
QString Version::str()
{
    QString s = QString("Version: %1.%2.%3.%4_%5%6%7")
            .arg((int)_major)
            .arg((int)_micro)
            .arg((int)_minor)
            .arg((int)_build)
            .arg((int)_year + 2000, 4, 10)
            .arg((int)_month, 2, 10, QChar('0'))
            .arg((int)_day, 2, 10, QChar('0'));
    switch (_identifier) {
        case Identifier::None:
            break;
        case Identifier::Demo:
            s += QString("(Demo)");
            break;
        case Identifier::Alpha:
            s += QString("(Alpha)");
            break;
        case Identifier::Beta:
            s += QString("(Beta)");
            break;
        case Identifier::Gamma:
            s += QString("(Gamma)");
            break;
        case Identifier::Release:
            s += QString("(Release)");
            break;
    }
    return s;
}
