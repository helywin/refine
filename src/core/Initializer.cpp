/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/16
 * @brief 软件初始化
 * @details
 ******************************************************************************/

#include "Initializer.hpp"

const char *Initializer::PATH_MAIN =
        "refine.ini";

const char *Initializer::VALUE[KEY_LENGTH][2] = {
        //CORE
        {"DATA_DIR",    "data/"},
        {"TEMP_DIR",    "temp/"},
        {"NAME_FORMAT", "yyyy-MM-dd@hh-mm-ss"},
        //GUI
        {"FPS_MSEC",    "10"}
};

const char *Initializer::GROUP_KEY[2] = {
        "CORE",
        "GUI"
};

const int Initializer::GROUP_SEGMENT[2] = {
        0,
        3
};


Initializer::Initializer() :
        _main(PATH_MAIN, QSettings::IniFormat) {}

/*!
 * @brief 读取配置，可以处理配置丢失的问题，设置了默认的配置
 */
void Initializer::loadSettings()
{
    _main_setting.clear();
    QStringList groups = _main.childGroups();
    qDebug() << groups;
    for (int i = GroupIndex::GroupStart; i <= GroupIndex::GroupEnd; ++i) {
        QString group = QString(GROUP_KEY[i]);
        qDebug() << "i: " << i;
        _main.beginGroup(group);
        qDebug() << "begin: " << group;
        _main_setting.insert(group, KeyMap());
        int start = GROUP_SEGMENT[i];
        int end;
        if (i == GroupIndex::GroupEnd) {
            end = KEY_LENGTH - 1;
        } else {
            end = GROUP_SEGMENT[i + 1] - 1;
        }
        for (int j = start; j <= end; ++j) {
            QString key = VALUE[j][KeyTab::KeyName];
            QString val = VALUE[j][KeyTab::DefaultValue];
            if (_main.contains(key)) {
                qDebug() << "contains: " << key;
                _main_setting[group]
                        .insert(key, _main.value(key).toString());
            } else {
                qDebug() << "not contains: " << key;
                _main_setting[group].insert(key, val);
            }
        }
        _main.endGroup();
    }
    qDebug() << str();
}

void Initializer::dumpSettings()
{
    _main.clear();
    for (const auto &group : _main_setting.keys()) {
        _main.beginGroup(group);
        for (const auto &key : _main_setting[group].keys()) {
            _main.setValue(key, _main_setting[group][key].toString());
        }
        _main.endGroup();
    }
}

QStringList Initializer::str() const
{
    QStringList list;
    for (const auto &group : _main_setting.keys()) {
        list.append(QString("[%1]").arg(group));
        for (const auto &key : _main_setting[group].keys()) {
            list.append(
                    QString("%1=%2")
                            .arg(key)
                            .arg(_main_setting[group][key].toString())
            );
        }
    }
    return list;
}