//
// Created by jiang.wenqiang on 2018/6/29.
//

#include "Curve.h"
#include "Log.h"

//Cell::Cell() {
//}
//
//Cell::Cell(const Cell &cells) {
//    *this = cells;
//}
//
//Cell& Cell::operator=(const Cell &cells) {
//    this->m_index = cells.m_index;
//    this->name = cells.name;
//    this->remark = cells.remark;
//    this->type = cells.type;
//    this->physical = cells.unit;
//    this->width = cells.width;
//    this->color = cells.color;
//    this->can_id = cells.can_id;
//    this->zero_flag = cells.zero_flag;
//    this->zero_bit = cells.zero_bit;
//    this->high_flag = cells.high_flag;
//    this->high_range[0] = cells.high_range[0];
//    this->high_range[1] = cells.high_range[1];
//    this->low_range[0] = cells.low_range[0];
//    this->low_range[1] = cells.low_range[1];
//    this->sample_type = cells.sample_type;
//    this->sample = cells.sample;
//    this->frame = cells.frame;
//    this->range_in[0] = cells.range_in[0];
//    this->range_in[1] = cells.range_in[1];
//    this->range_out[0] = cells.range_out[0];
//    this->range_out[1] = cells.range_out[1];
//    this->logic = cells.logic;
//}

bool Curve::Cell::check() {
    bool flag = true;
    qInfo() << "检查曲线:" << name;
    if (name.isEmpty()) {
        qWarning("名称为空");
        name = QString("曲线%1").arg(index);
    }
    if (width > 10) {
        qWarning("线太宽");
        width = 1;
    }
    if (color > 0xFFFFFF) {
        qWarning("颜色超范围");
        color &= 0x00FFFFFF;
    }
    if (can_id > 0x7FF) {
        qCritical("CAN_ID不对");
        flag = false;
    }
    if (high_flag) {
        if (high_byte > 7) {
            qCritical("高字节超过7");
            flag = false;
        }
        if (high_range[0] > 7 || high_range[1] > 7) {
            qCritical("高位范围不对");
            flag = false;
        }
        if (high_range[0] > high_range[1]) {
            unsigned short temp = high_range[0];
            high_range[0] = high_range[1];
            high_range[1] = temp;
            qWarning("高位范围弄反");
        }
    }
    if (low_byte > 7) {
        qCritical("低字节超过7");
        flag = false;
    }
    if (zero_flag && low_byte == 0) {
        qCritical("低字节和零字节冲突");
        flag = false;
    }
    if (low_range[0] > 7 || low_range[1] > 7) {
        qCritical("低位范围不对");
        flag = false;
    }
    if (low_range[0] > low_range[1]) {
        unsigned short temp = low_range[0];
        low_range[0] = low_range[1];
        low_range[1] = temp;
        qWarning("低位范围弄反");
    }
    if (range_in[0] > range_in[1]) {
        long temp = range_in[0];
        range_in[0] = range_in[1];
        range_in[1] = temp;
        qWarning("输入范围弄反");
    }
    if (range_out[0] > range_out[1]) {
        long temp = range_out[0];
        range_out[0] = range_out[1];
        range_out[1] = temp;
        qWarning("输出范围弄反");
    }
    qInfo("检查结束");
    return flag;
}

QString Curve::Cell::str() {
    QString s;
    QStringList list;
    list.append(QString::number(index));
    list.append(name);
    list.append(remark);
    if (type == Cell::Logical) {
        list.append(QString("逻辑"));
    } else {
        list.append(QString("物理"));
    }
    list.append(unit);
    list.append(QString::number(width));
    list.append(QString("0x%1").arg(color, 6, 16, QLatin1Char('0')));
    list.append(QString("0x%1").arg(can_id, 3, 16, QLatin1Char('0')));
    if (zero_flag) {
        list.append(QString("有;") + QString::number(zero_byte));
    } else {
        list.append(QString("无;") + QString::number(zero_byte));
    }
    if (high_flag) {
        s = QString("有;");
    } else {
        s = QString("无;");
    }
    list.append(s + QString("%1;%2~%3")
            .arg(high_byte)
            .arg(high_range[0])
            .arg(high_range[1]));
    list.append(QString("%1;%2~%3")
                        .arg(low_byte)
                        .arg(low_range[0])
                        .arg(low_range[1]));
    if (sample_type == Curve::Cell::Frame) {
        list.append(QString("帧数;%1").arg(sample));
    } else {
        list.append(QString("时间;%1").arg(sample));
    }
    list.append(QString("%1~%2")
                        .arg(range_in[0])
                        .arg(range_in[1]));
    list.append(QString("%1~%2")
                        .arg(range_out[0])
                        .arg(range_out[1]));
    s = list.join(QChar(','));
    return s;
}

Curve::Curve() {
    _status = Curve::Empty;
}

bool Curve::loadCsv(QFile &csv) {
    if (!csv.isOpen()) {
        csv.open(QIODevice::ReadOnly | QIODevice::Text);
    } else {
        csv.close();
        csv.open(QIODevice::ReadOnly | QIODevice::Text);
    }
    if (!csv.isOpen()) {
        qCritical("文件打开失败");
        _status = Curve::File;
        return false;
    }
    QTextStream stream(&csv);
    stream.setCodec("gbk");
    QString str;
    str = stream.readLine(200);
    str = str.simplified();
    str.remove(QRegExp("\\s"));
    _header = str.split(QChar(','), QString::KeepEmptyParts);
    while (!stream.atEnd()) {
        str = stream.readLine(200);
        str = str.simplified();
        str.remove(QRegExp("\\s"));
        QStringList list = str.split(QChar(','), QString::KeepEmptyParts);
        QStringList split_list;
        Cell cell;
        unsigned short i = 0;
        bool first = true;
        bool flag = false;
        for (const auto &iter : _header) {
            if (!first) {
                i += 1;
            } else {
                first = false;
            }
            if (iter == "序号") {
                flag = false;
                cell.index = list[i].toUShort(&flag, 10);
                if (!flag) {
                    cell.index = i;
                    cell.index += 1;
                    qWarning("读取序号失败");
                }
                continue;
            }
            if (iter == "名称") {
                cell.name = list[i];
                continue;
            }
            if (iter == "备注") {
                cell.remark = list[i];
                continue;
            }
            if (iter == "类型") {
                if (list[i] == "逻辑") {
                    cell.type = Cell::Logical;
                } else if (list[i] == "物理") {
                    cell.type = Cell::Physical;
                } else {
                    qWarning("未知类型,设为物理");
                    cell.type = Cell::Physical;
                }
                continue;
            }
            if (iter == "物理") {
                cell.unit = list[i];
                continue;
            }
            if (iter == "宽度") {
                flag = false;
                cell.width = list[i].toUShort(&flag, 10);
                if (!flag) {
                    qWarning("读取宽度失败,设为1");
                    cell.width = 1;
                }
                continue;
            }
            if (iter == "颜色") {
                flag = false;
                cell.color = list[i].toULong(&flag, 0);
                if (!flag) {
                    cell.color = 0x000000;
                    qWarning("读取颜色失败,设置为 0x000000");
                }
                continue;
            }
            if (iter == "地址") {
                flag = false;
                cell.can_id = list[i].toULong(&flag, 0);
                if (!flag) {
                    qCritical("读取地址失败");
                    _status = Curve::Error;
                    csv.close();
                    return false;
                }
            }
            if (iter == "零字节") {
                split_list = list[i].split(QChar(';'));
                if (split_list[0] == "无") {
                    cell.zero_flag = false;
                    cell.zero_byte = 0;
                    continue;
                } else if (split_list[0] == "有") {
                    cell.zero_flag = true;
                    flag = false;
                    cell.zero_byte = split_list[1].toUShort(&flag, 10);
                    if (!flag) {
                        qCritical("零位无法读取");
                        _status = Curve::Error;
                        csv.close();
                        return false;
                    }
                }
            }
            if (iter == "高字节") {
                split_list = list[i].split(QChar(';'));
                if (split_list[0] == "有") {
                    cell.high_flag = true;
                    flag = false;
                    cell.high_byte = split_list[1].toUShort(&flag, 10);
                    split_list = split_list[2].split(QChar('~'));
                    flag = false;
                    cell.high_range[0] = split_list[0].toUShort(&flag, 10);
                    if (flag) {
                        flag = false;
                        cell.high_range[1] = split_list[1].toUShort(&flag, 10);
                    }

                    if (!flag) {
                        qCritical("读取高位范围失败");
                        _status = Curve::Error;
                        csv.close();
                        return false;
                    } else {
                        continue;
                    }

                } else if (split_list[0] == "无") {
                    cell.high_flag = false;
                    cell.high_byte = 0;
                    cell.high_range[0] = 0;
                    cell.high_range[1] = 0;
                    continue;
                } else {
                    qWarning("无法识别,设置为无");
                    cell.high_flag = false;
                    cell.high_byte = 0;
                    cell.high_range[0] = 0;
                    cell.high_range[1] = 0;
                    continue;
                }
            }
            if (iter == "低字节") {
                split_list = list[i].split(QChar(';'));
                flag = false;
                cell.low_byte = split_list[0].toUShort(&flag, 10);
                split_list = split_list[1].split(QChar('~'));
                cell.low_range[0] = split_list[0].toUShort(&flag, 10);
                if (flag) {
                    cell.low_range[1] = split_list[1].toUShort(&flag, 10);
                }
                if (!flag) {
                    qCritical("读取低位范围失败");
                    _status = Curve::Error;
                    csv.close();
                    return false;
                } else {
                    continue;
                }
            }
            if (iter == "采样") {
                split_list = list[i].split(QChar(';'));
                flag = false;
                cell.sample = split_list[1].toUShort(&flag, 10);
                if (split_list[0] == "帧数") {
                    cell.sample_type = Cell::Frame;
                } else if (split_list[0] == "时间") {
                    cell.sample_type = Cell::Time;
                } else {
                    cell.sample_type = Cell::Time;
                    qWarning("读取失败,设为时间采样");
                }
                continue;
            }
            if (iter == "输入量程") {
                split_list = list[i].split(QChar('~'));
                flag = false;
                cell.range_in[0] = split_list[0].toUShort(&flag, 10);
                if (flag) {
                    cell.range_in[1] = split_list[1].toUShort(&flag, 10);
                }
                if (!flag) {
                    qCritical("读取输入量程失败");
                    _status = Curve::Error;
                    csv.close();
                    return false;
                } else {
                    continue;
                }
            }
            if (iter == "输出量程") {
                split_list = list[i].split(QChar('~'));
                flag = false;
                cell.range_out[0] = split_list[0].toUShort(&flag, 10);
                if (flag) {
                    cell.range_out[1] = split_list[1].toUShort(&flag, 10);
                }
                if (!flag) {
                    qCritical("读取输出量程失败");
                    _status = Curve::Error;
                    csv.close();
                    return false;
                } else {
                    continue;
                }
            }
        }
        if (cell.check()) {
            _config.append(cell);
        } else {
            qCritical("曲线配置有问题");
            _status = Curve::Error;
            csv.close();
            return false;
        }
    }
    csv.close();
    _status = Curve::Ok;
    return true;
}

Curve::Status Curve::status() const {
    return _status;
}

void Curve::str(QStringList &list) {
    list.append(_header.join(QChar(',')));
    for (auto &cell : _config) {
        list.append(cell.str());
    }
}

const Curve::Cell &Curve::operator[](unsigned int index) {
    return _config.at(index);
}

bool Curve::transform(unsigned long id, unsigned char *data,
                            double &result, unsigned short &index) {
    for (const auto &iter : _config) {
        bool flag = iter.can_id == id &&
                    (!iter.zero_flag || data[0] == iter.zero_byte);
        if (flag) {
            unsigned int full;
            unsigned int high_byte;
            unsigned int low_byte;
            if (iter.high_flag) {
                high_byte = data[iter.high_byte];
                high_byte <<= 7 - iter.high_range[1];
                high_byte >>= 7 - iter.high_range[1] + iter.high_range[0];
                high_byte <<= iter.low_range[1] - iter.low_range[0] + 1;
            } else {
                high_byte = 0;
            }
            low_byte = data[iter.low_byte];
//            qDebug() << "low byte " << low_byte;
            low_byte <<= 7 - iter.low_range[1];
            low_byte >>= 7 - iter.low_range[1] + iter.low_range[0];
            full = high_byte + low_byte;
//            qDebug() << full;
            double k;
            double b;
            k = (double) (iter.range_out[1] - iter.range_out[0]) /
                (double) (iter.range_in[1] - iter.range_in[0]);
            b = (double) iter.range_out[0] - k * iter.range_in[0];
            result = (double) full * k + b;
            index = iter.index;
            return true;
        } else {
            return false;
        }
    }
}

void Curve::names(QStringList &list) {
    list.clear();
    for (const auto &iter : _config) {
        list.append(iter.name);
    }
}