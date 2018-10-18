/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/16
 * @brief 软件初始化
 * @details
 ******************************************************************************/

#ifndef REFINE_INITIALIZER_HPP
#define REFINE_INITIALIZER_HPP

#include <QtCore/QSettings>
#include <QtCore/QMap>
#include <QtCore/QDebug>

/*!
 * @brief 程序初始化类，比如加载配置文件，初始环境配置，文件清理等等
 */

class Initializer
{
public:
    typedef QMap<QString, QVariant> KeyMap;
    typedef QMap<QString, KeyMap> SettingsMap;

#define KEY_LENGTH 3

    static const char *VALUE[KEY_LENGTH][2];
    static const char *PATH_MAIN;
    static const char *GROUP_NAME[2];
    static const int GROUP_SEGMENT[2];

    enum GroupIndex
    {
        GroupStart = 0,
        Core = GroupStart,
        Gui,
        GroupEnd = Gui
    };

    enum KeyTab
    {
        KeyName = 0,
        DefaultValue
    };

    enum KeyIndex
    {
        IndexStart = 0,
        TempDir = IndexStart,
        NameFormat,
        IndexEnd = NameFormat
    };


public:
    const QString path_main = QString();
    const QString group_CORE = QString("CORE");
    const QString key_CORE_TEMP_DIR = QString("TEMP_DIR");
    const QString def_CORE_TEMP_DIR = QString("./temp/");
    const QString key_CORE_NAME_FORMAT = QString("NAME_FORMAT");
    const QString def_CORE_NAME_FORMAT = QString("yyyy-MM-dd hh:mm:ss");

private:
    QSettings _main;
    SettingsMap _main_setting;

public:
    Initializer();

    void loadSettings();

    void dumpSettings();

    QStringList str() const;
};


#endif //REFINE_INITIALIZER_HPP
