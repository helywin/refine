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

#define KEY_LENGTH 4

    static const char *VALUE[KEY_LENGTH][2];
    static const char *PATH_MAIN;
    static const char *GROUP_KEY[2];
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
        DataDir = IndexStart,
        TempDir,
        NameFormat,
        IndexEnd = NameFormat
    };

private:
    QSettings _main;
    SettingsMap _main_setting;

public:
    Initializer();

    inline SettingsMap &map() { return _main_setting; }

    inline const SettingsMap &map() const { return _main_setting; }

    void loadSettings();

    void dumpSettings();

    inline QVariant get(GroupIndex group, KeyIndex key)
    {
        return _main_setting[GROUP_KEY[group]][VALUE[key][KeyName]];
    }

    inline void set(GroupIndex group, KeyIndex key, const QVariant &var)
    {
        _main_setting[GROUP_KEY[group]][VALUE[key][KeyName]] = var;
    }


    QStringList str() const;
};


#endif //REFINE_INITIALIZER_HPP
