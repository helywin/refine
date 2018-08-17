//
// Created by jiang.wenqiang on 2018/8/6.
//

#include "Curve.h"
#include "Csv.h"
#include <QtCore/QDebug>

Curve::Curve() : _status(Status::Uninitialized) {}

bool Curve::load(QFile &f, const Curve::FileType type) {
    bool flag = false;
    switch (type) {
        case Curve::FileType::Csv:
            flag = loadFromCsv(f);
            break;
        case Curve::FileType::Sqlite3:
            flag = loadFromSqlite3(f);
            break;
    }
    return flag;
}

bool Curve::dump(QFile &f, const Curve::FileType type) const {
    bool flag = false;
    switch (type) {
        case Curve::FileType::Csv:
            flag = dumpToCsv(f);
            break;
        case Curve::FileType::Sqlite3:
            flag = dumpToSqlite3(f);
            break;
    }
    return flag;
}

QStringList Curve::header() const {
    return _header;
}

int Curve::size() const {
    return _cells.size();
}

QStringList Curve::str() const {
    QStringList list;
    for (const auto &cell : _cells) {
        list.append(cell.str());
    }
    return qMove(list);
}

void Curve::appendRow() {
    _cells.append(Cell(_cells.size() + 1));
    _header.append(QString("未命名"));
}

void Curve::appendRow(const Bundle bundle) {
    _cells.append(Cell(_cells.size() + 1, bundle));
    switch (bundle) {
        case Bundle::Acceleration:
            _header.append(QString("加速度"));
            break;
        default:
            break;
    }
}

void Curve::insertRow(const int index) {
    if (index > _cells.size()) {
        _cells.insert(_cells.size(), Cell(_cells.size() + 1));
    } else if (index >= 0) {
        _cells.insert(index, Cell(index));
    }
}

void Curve::insertRow(const int index, const Bundle bundle) {
    if (index > _cells.size()) {
        _cells.insert(_cells.size(), Cell(_cells.size() + 1, bundle));
    } else if (index >= 0) {
        _cells.insert(index, Cell(index, bundle));
    }
}

void Curve::removeRow(const int index) {
    if (index > _cells.size()) {
        _cells.remove(_cells.size());
    } else if (index >= 0) {
        _cells.remove(index);
    }
}

void Curve::removeAllRow() {
    _cells.clear();
}

bool Curve::loadFromCsv(QFile &f) {
    if (f.isOpen()) {
        f.close();
    }
    ::Csv csv;
    csv.setFile(&f);
    if (!csv.startRead("gbk")) {
        qCritical("文件打开失败");
        return false;
    }
    csv.readLine(_header);
    while (!csv.finishRead()) {
        QStringList list;
        csv.readLine(list);
        unsigned short i = 0;
        Cell cell(i);
        bool first = true;
        for (const auto &iter : _header) {
            if (!first) {
                i += 1;
            } else {
                first = false;
            }
            if (iter == "序号") {
                cell.setIndexByStr(list[i]);
                continue;
            }
            if (iter == "显示") {
                cell.setDisplayByStr(list[i]);
                continue;
            }
            if (iter == "名称") {
                cell.setNameByStr(list[i]);
                continue;
            }
            if (iter == "备注") {
                cell.setRemarkByStr(list[i]);
                continue;
            }
            if (iter == "类型") {
                cell.setTypeByStr(list[i]);
                continue;
            }
            if (iter == "单位") {
                cell.setUnitByStr(list[i]);
                continue;
            }
            if (iter == "宽度") {
                cell.setWidthByStr(list[i]);
                continue;
            }
            if (iter == "颜色") {
                cell.setColorByStr(list[i]);
                continue;
            }
            if (iter == "地址") {
                cell.setCanIdByStr(list[i]);
                continue;
            }
            if (iter == "零字节") {
                cell.setZeroByteByStr(list[i]);
                continue;
            }
            if (iter == "高字节") {
                cell.setHighByteByStr(list[i]);
                continue;
            }
            if (iter == "低字节") {
                cell.setLowByteByStr(list[i]);
                continue;
            }
            if (iter == "采样") {
                cell.setSampleByStr(list[i]);
                continue;
            }
            if (iter == "输入量程") {
                cell.setRangeInByStr(list[i]);
                continue;
            }
            if (iter == "输出量程") {
                cell.setRangeOutByStr(list[i]);
                continue;
            }
        }
        if (cell.check()) {
            _cells.append(cell);
        } else {
            qCritical("曲线配置有问题");
            f.close();
            return false;
        }
    }
    _status = Status::Initialized;
    return true;
}

bool Curve::loadFromSqlite3(QFile &f) {
    return false;
}

bool Curve::dumpToCsv(QFile &f) const {
    if (f.isOpen()) {
        f.close();
    }
    ::Csv csv;
    csv.setFile(&f);
    if (!csv.startWrite("gbk")) {
        qCritical("文件打开失败");
        return false;
    }
    csv.writeLine(header());
    for (const auto &cell : _cells) {
        csv.writeLine(cell.str());
    }
    return csv.finishWrite();
}

bool Curve::dumpToSqlite3(QFile &f) const {
    return false;
}

Curve::Cell &Curve::operator[](const int index) {
    Q_ASSERT(index >= 0 && index < _cells.size());
    return _cells[index];
}


Curve::Cell::Cell(int index) {
    Q_ASSERT(index > 0);
    _index = (unsigned short) (index - 1);
    _display = false;
    _name = QString("未命名");
    _type = Type::Physical;
    _unit = QString("无单位");
    _width = 1;
    _color = 0xFFFFFF;
    _can_id = 0x777;
    _zero_byte_existed = false;
    _zero_byte = 0;
    _high_byte_existed = false;
    _high_byte = 0;
    _high_byte_range[0] = 0;
    _high_byte_range[1] = 0;
    _low_byte = 0;
    _low_byte_range[0] = 0;
    _low_byte_range[1] = 7;
    _sample_type = Sample::Timed;
    _sample = 10;
    _range_in[0] = 0;
    _range_in[1] = 100;
    _range_out[0] = 0;
    _range_out[1] = 100;
//    _logic_map = ;
    _remark = QString("无");
    _bundle = Bundle::None;
}

Curve::Cell::Cell(int index, Curve::Bundle bundle) : Cell(index) {
    switch (bundle) {
        case Bundle::Acceleration:
            _name = QString("加速度");
            _unit = QString("m/s^2");
            _remark = QString("汽车的加速度");
            break;
        default:
            break;
    }
}

bool Curve::Cell::check() const {
    return false;
}

QStringList Curve::Cell::str() const {
    QStringList list;
    list.append(indexStr());
    list.append(displayStr());
    list.append(nameStr());
    list.append(typeStr());
    list.append(unitStr());
    list.append(widthStr());
    list.append(colorStr());
    list.append(canIdStr());
    list.append(zeroByteStr());
    list.append(highByteStr());
    list.append(lowByteStr());
    list.append(sampleStr());
    list.append(rangeInStr());
    list.append(rangeOutStr());
    list.append(logicMapStr());
    list.append(remarkStr());
    return qMove(list);
}

//str getter

QString Curve::Cell::indexStr() const {
    return QString::number(_index);
}

QString Curve::Cell::displayStr() const {
    return QString::number(_display);
}

QString Curve::Cell::nameStr() const {
    return QString(_name);
}

QString Curve::Cell::typeStr() const {
    switch (_type) {
        case Type::Physical:
            return QString("物理");
        case Type::Logical:
            return QString("逻辑");
        default:
            return QString();
    }
}

QString Curve::Cell::unitStr() const {
    return QString(_unit);
}

QString Curve::Cell::widthStr() const {
    return QString::number(_width);
}

QString Curve::Cell::colorStr() const {
    return QString("0x%1").arg(_color, 6, 16, QChar('0'));
}

QString Curve::Cell::canIdStr() const {
    return QString("0x%1").arg(_can_id, 3, 16, QChar('0'));
}

QString Curve::Cell::zeroByteStr() const {
    if (_zero_byte_existed) {
        return QString("有;%1").arg(_zero_byte);
    } else {
        return QString("无");
    }
}

QString Curve::Cell::highByteStr() const {
    if (_high_byte_existed) {
        return QString("有;%1;%2~%3")
                .arg(_high_byte)
                .arg(_high_byte_range[0])
                .arg(_high_byte_range[1]);
    } else {
        return QString("无");
    }
}

QString Curve::Cell::lowByteStr() const {
    return QString("%1;%2~%3")
            .arg(_low_byte)
            .arg(_low_byte_range[0])
            .arg(_low_byte_range[1]);
}

QString Curve::Cell::sampleStr() const {
    switch (_sample_type) {
        case Sample::Timed:
            return QString("时间;%1").arg(_sample);
        case Sample::Framed:
            return QString("帧数;%1").arg(_sample);
        default:
            return QString();
    }
}

QString Curve::Cell::rangeInStr() const {
    return QString("%1~%2")
            .arg(_range_in[0])
            .arg(_range_in[1]);
}

QString Curve::Cell::rangeOutStr() const {
    return QString("%1~%2")
            .arg(_range_out[0])
            .arg(_range_out[1]);
}

QString Curve::Cell::logicMapStr() const {
    if (_type == Type::Physical) {
        return QString("非逻辑变量");
    } else {
        QStringList list;
        for (const auto &iter : _logic_map.keys()) {
            list.append(QString("%1:%2").arg(iter).arg(_logic_map[iter]));
        }
        return qMove(list.join(QChar(';')));
    }
}

QString Curve::Cell::remarkStr() const {
    return QString(_remark);
}

//val getter

unsigned short Curve::Cell::index() const {
    return _index;
}

bool Curve::Cell::display() const {
    return _display;
}

QString Curve::Cell::name() const {
    return _name;
}

Curve::Cell::Type Curve::Cell::type() const {
    return _type;
}

QString Curve::Cell::unit() const {
    return _unit;
}

unsigned short Curve::Cell::width() const {
    return _width;
}

unsigned long Curve::Cell::color() const {
    return _color;
}

unsigned long Curve::Cell::canId() const {
    return _can_id;
}

bool Curve::Cell::zeroByteExisted() const {
    return _zero_byte_existed;
}

unsigned short Curve::Cell::zeroByte() const {
    return _zero_byte;
}

bool Curve::Cell::highByteExisted() const {
    return _high_byte_existed;
}

unsigned short Curve::Cell::highByte() const {
    return _high_byte;
}

const unsigned short *Curve::Cell::highByteRange() const {
    return _high_byte_range;
}

unsigned short Curve::Cell::lowByte() const {
    return _low_byte;
}

const unsigned short *Curve::Cell::lowByteRange() const {
    return _low_byte_range;
}

Curve::Cell::Sample Curve::Cell::sampleType() const {
    return _sample_type;
}

unsigned long Curve::Cell::sample() const {
    return _sample;
}

const long *Curve::Cell::rangeIn() const {
    return _range_in;
}

const long *Curve::Cell::rangeOut() const {
    return _range_out;
}

QMap<unsigned int, QString> Curve::Cell::logicMap() const {
    return _logic_map;
}

QString Curve::Cell::remark() const {
    return _remark;
}

//str setter

void Curve::Cell::setIndexByStr(QString &s) {
    _index = s.toUShort();
}

void Curve::Cell::setDisplayByStr(QString &s) {
    _display = s == QString("是");
}

void Curve::Cell::setNameByStr(QString &s) {
    _name = s;
}

void Curve::Cell::setTypeByStr(QString &s) {
    if (s == QString("物理")) {
        _type = Type::Physical;
    } else {
        _type = Type::Logical;
    }
}

void Curve::Cell::setUnitByStr(QString &s) {
    _unit = s;
}

void Curve::Cell::setWidthByStr(QString &s) {
    _width = s.toUShort();
}

void Curve::Cell::setColorByStr(QString &s) {
    _color = s.toULong(nullptr, 16);
}

void Curve::Cell::setCanIdByStr(QString &s) {
    _can_id = s.toULong(nullptr, 16);
}

void Curve::Cell::setZeroByteByStr(QString &s) {
    if (s == QString("无")) {
        _zero_byte_existed = false;
    } else {
        _zero_byte_existed = true;
        QStringList list = s.split(QChar(';'));
        _zero_byte = list[1].toUShort();
    }
}

void Curve::Cell::setHighByteByStr(QString &s) {
    if (s == QString("无")) {
        _high_byte_existed = false;
    } else {
        _high_byte_existed = true;
        QStringList list = s.split(QChar(';'));
        _high_byte = list[1].toUShort();
        list = list[2].split(QChar('~'));
        _high_byte_range[0] = list[0].toUShort();
        _high_byte_range[1] = list[1].toUShort();
    }
}

void Curve::Cell::setLowByteByStr(QString &s) {
    QStringList list = s.split(QChar(';'));
    _low_byte = list[0].toUShort();
    list = list[1].split(QChar('~'));
    _low_byte_range[0] = list[0].toUShort();
    _low_byte_range[1] = list[1].toUShort();
}

void Curve::Cell::setSampleByStr(QString &s) {
    QStringList list = s.split(QChar(';'));
    if (list[0] == QString("帧数")) {
        _sample_type = Sample::Framed;
    } else {
        _sample_type = Sample::Timed;
    }
    _sample = list[1].toULong();
}

void Curve::Cell::setRangeInByStr(QString &s) {
    QStringList list = s.split(QChar(';'));
    _range_in[0] = list[0].toLong();
    _range_in[1] = list[1].toLong();
}

void Curve::Cell::setRangeOutByStr(QString &s) {
    QStringList list = s.split(QChar(';'));
    _range_out[0] = list[0].toLong();
    _range_out[1] = list[1].toLong();
}

void Curve::Cell::setLogicMapByStr(QString &s) {

}

void Curve::Cell::setRemarkByStr(QString &s) {
    _remark = s;
}

void Curve::Cell::setIndexByVal(const unsigned short v) {
    _index = v;
}

void Curve::Cell::setDisplayByVal(const bool v) {
    _display = v;
}

void Curve::Cell::setNameByVal(const QString &v) {
    _name = v;
}

void Curve::Cell::setTypeByVal(const Curve::Cell::Type v) {
    _type = v;
}

void Curve::Cell::setUnitByVal(const QString &v) {
    _unit = v;
}

void Curve::Cell::setWidthByVal(const unsigned short v) {
    _width = v;
}

void Curve::Cell::setColorByVal(const unsigned long v) {
    _color = v;
}

void Curve::Cell::setCanIdByVal(const unsigned long v) {
    _can_id = v;
}

void Curve::Cell::setZeroByteExistedByVal(const bool v) {
    _zero_byte_existed = v;
}

void Curve::Cell::setZeroByteByVal(const unsigned short v) {
    _zero_byte = v;
}

void Curve::Cell::setHighByteExistedByVal(const bool v) {
    _high_byte_existed = v;
}

void Curve::Cell::setHighByteByVal(const unsigned short v) {
    _high_byte = v;
}

void Curve::Cell::setHighByteRangeByVal(const unsigned short *v) {
    _high_byte_range[0] = v[0];
    _high_byte_range[1] = v[1];
}

void Curve::Cell::setLowByteByVal(const unsigned short v) {
    _low_byte = v;
}

void Curve::Cell::setLowByteRangeByVal(const unsigned short *v) {
    _low_byte_range[0] = v[0];
    _low_byte_range[1] = v[1];
}

void Curve::Cell::setSampleTypeByVal(const Curve::Cell::Sample v) {
    _sample_type = v;
}

void Curve::Cell::setSampleByVal(const unsigned long v) {
    _sample = v;
}

void Curve::Cell::setRangeInByVal(const long *v) {
    _range_in[0] = v[0];
    _range_in[1] = v[1];
}

void Curve::Cell::setRangeOutByVal(const long *v) {
    _range_out[0] = v[0];
    _range_out[1] = v[1];
}

void Curve::Cell::setLogicMapByVal(const Logic &v) {
    _logic_map = v;
}

void Curve::Cell::setRemarkByVal(const QString &v) {
    _remark = v;
}


