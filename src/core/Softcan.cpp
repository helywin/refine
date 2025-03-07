/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/4
 * @brief Softcan配置读取
 * @details 实现读取Softcan配置并转换为Curve配置
 ******************************************************************************/

#include "Softcan.hpp"

Softcan::Softcan(Message *message) :
        Message(message), _cells(), _head(-1) {}

bool Softcan::load(QFile &file)
{
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen()) {
        return false;
    }
    clear();
    QDataStream stream;
    stream.setByteOrder(QDataStream::ByteOrder::LittleEndian);
    stream.setDevice(&file);
    char magic[SOFTCAN_MAGIC_LEN];
    for (int i = 0; i < 50; ++i) {
        stream.device()->seek(i);
        stream.readRawData(magic, SOFTCAN_MAGIC_LEN);
        if (checkMagic(magic)) {
            break;
        }
    }


    stream >> *this;

    stream.unsetDevice();
    file.close();
    return true;
}

QStringList Softcan::tab() const
{
    QStringList list;
    list.append(QString("序号"));
    list.append(QString("名称"));
    list.append(QString("物理意义"));
    list.append(QString("物理单位"));
    list.append(QString("CANID"));
    list.append(QString("零字节"));
    list.append(QString("高字节"));
    list.append(QString("低字节"));
    list.append(QString("高位"));
    list.append(QString("低位"));
    list.append(QString("输入范围"));
    list.append(QString("颜色"));
    list.append(QString("宽度"));
    list.append(QString("可视"));
    list.append(QString("采样模式"));
    list.append(QString("采样值"));
    list.append(QString("样式"));
    list.append(QString("允许更新"));
    list.append(QString("横轴大小"));
    list.append(QString("纵轴大小"));
    list.append(QString("横轴最大"));
    list.append(QString("横轴最小"));
    list.append(QString("纵轴最大"));
    list.append(QString("纵轴最小"));
    list.append(QString("横轴临时"));
    list.append(QString("纵轴临时"));
    list.append(QString("简介"));
    list.append(QString("保存标志"));
    return list;
}

void Softcan::add()
{
    _cells.append(Cell());
    _head += 1;
}

Softcan::Cell::Cell()
{
    _index = 0;
    _can_id = 0;
    _zero_byte = 0;
    _high_byte = 0;
    _low_byte = 0;
    _first_bit = 0;
    _second_bit = 0;
    _third_bit = 0;
    _input_max = 0;
    _input_min = 0;
    _color = 0;
    _width = 0;
    _visible = 0;
    _sample_mode = 0;
    _sample_num = 0;
    _style = 0;
    _enable_update = 0;
    _size_x = 0;
    _size_y = 0;
    _x_max = 0;
    _x_min = 0;
    _y_max = 0;
    _y_min = 0;
    _x_temp = 0;
    _y_temp = 0;
//    _analog_mean = 0;
//    _unit = 0;
//    _intro = 0;
//    _name = 0;
    _save_data = 0;
}

QStringList Softcan::Cell::str() const
{
    QStringList list;
    list.append(QString("序号:%1").arg(_index));
    list.append(QString("名称:") + _name);
    list.append(QString("物理意义:") + _analog_mean);
    list.append(QString("物理单位:") + _unit);
    list.append(QString("CANID:0x%1").arg(_can_id, 3, 16, QChar('0')));
    list.append(QString("零字节:%1").arg(_zero_byte));
    list.append(QString("高字节:%1").arg(_high_byte - 2));
    list.append(QString("低字节:%1").arg(_low_byte));
    list.append(QString("高字节位:%1").arg(_first_bit));
    list.append(QString("低字节为:%1~%2").arg(_third_bit).arg(_second_bit));
    list.append(QString("输入范围:%1~%2").arg(_input_min).arg(_input_max));
    list.append(QString("颜色:0x%1").arg(_color, 6, 16, QChar('0')));
    list.append(QString("宽度:%1").arg(_width));
    list.append(QString("可视:%1").arg(_visible));
    list.append(QString("采样模式:%1").arg(_sample_mode));
    list.append(QString("采样值:%1").arg(_sample_num));
    list.append(QString("样式:%1").arg(_style));
    list.append(QString("允许更新:%1").arg(_enable_update));
    list.append(QString("横轴大小:%1").arg(_size_x));
    list.append(QString("纵轴大小:%1").arg(_size_y));
    list.append(QString("横轴最大:%1").arg(_x_max));
    list.append(QString("横轴最小:%1").arg(_x_min));
    list.append(QString("纵轴最大:%1").arg(_y_max));
    list.append(QString("纵轴最小:%1").arg(_y_min));
    list.append(QString("横轴临时:%1").arg(_x_temp));
    list.append(QString("纵轴临时:%1").arg(_y_temp));
    list.append(QString("简介:") + _intro);
    list.append(QString("保存标志:%1").arg(_save_data));
    return list;
}

QStringList Softcan::Cell::strClean() const
{
    QStringList list;
    list.append(QString("%1").arg(_index));
    list.append(_name);
    list.append(_analog_mean);
    list.append(_unit);
    list.append(QString("0x%1").arg(_can_id, 3, 16, QChar('0')));
    list.append(QString("%1").arg(_zero_byte));
    list.append(QString("%1").arg(_high_byte - 2));
    list.append(QString("%1").arg(_low_byte));
    list.append(QString("%1").arg(_first_bit));
    list.append(QString("%1~%2").arg(_third_bit).arg(_second_bit));
    list.append(QString("%1~%2").arg(_input_min).arg(_input_max));
    list.append(QString("0x%1").arg(_color, 6, 16, QChar('0')));
    list.append(QString("%1").arg(_width));
    list.append(QString("%1").arg(_visible));
    list.append(QString("%1").arg(_sample_mode));
    list.append(QString("%1").arg(_sample_num));
    list.append(QString("%1").arg(_style));
    list.append(QString("%1").arg(_enable_update));
    list.append(QString("%1").arg(_size_x));
    list.append(QString("%1").arg(_size_y));
    list.append(QString("%1").arg(_x_max));
    list.append(QString("%1").arg(_x_min));
    list.append(QString("%1").arg(_y_max));
    list.append(QString("%1").arg(_y_min));
    list.append(QString("%1").arg(_x_temp));
    list.append(QString("%1").arg(_y_temp));
    list.append(_intro);
    list.append(QString("%1").arg(_save_data));
    return list;
}

QString Softcan::Cell::readWString(QDataStream &stream)
{
    wchar_t str[40];
    memset(str, 0, sizeof(str));
    unsigned char char_buf;
    short short_buf;
    stream >> char_buf;
    stream >> char_buf;
    stream >> char_buf;
    stream >> char_buf;
    for (unsigned int i = 0; i < char_buf; ++i) {
        stream >> short_buf;
        str[i] = static_cast<wchar_t>(short_buf);
    }
    return QString::fromWCharArray(str, char_buf);
}

QDataStream &operator>>(QDataStream &stream, Softcan::Cell &cell)
{
    stream >> cell._index;
    stream >> cell._can_id;
    stream >> cell._zero_byte;
    stream >> cell._high_byte;
    stream >> cell._low_byte;
    stream >> cell._first_bit;
    stream >> cell._second_bit;
    stream >> cell._third_bit;
    stream >> cell._input_max;
    stream >> cell._input_min;
    unsigned int color;
    stream >> color;
    color = ((color & 0x0000ffu) << 16u)
            + (color & 0x00ff00u)
            + ((color & 0xff0000u) >> 16u);  //COLORREF转RGB
    cell._color = color;
    stream >> cell._width;
    stream >> cell._visible;
    stream >> cell._sample_mode;
    stream >> cell._sample_num;
    stream >> cell._style;
    stream >> cell._enable_update;
    stream >> cell._size_x;
    stream >> cell._size_y;
    stream >> cell._x_max;
    stream >> cell._x_min;
    stream >> cell._y_max;
    stream >> cell._y_min;
    stream >> cell._x_temp;
    stream >> cell._y_temp;
    cell._analog_mean = Softcan::Cell::readWString(stream);
    cell._unit = Softcan::Cell::readWString(stream);
    cell._intro = Softcan::Cell::readWString(stream);
    cell._name = Softcan::Cell::readWString(stream);
    stream >> cell._save_data;
    if (cell._save_data) {
        int len;
        stream >> len;
        for (int i = 0; i < len; ++i) {
            double v;
            stream >> v;
            cell._x.append(v);
            if (stream.atEnd()) {
                return stream;
            }
            stream >> v;
            cell._y.append(v);
            if (stream.atEnd()) {
                return stream;
            }
        }
    }
    stream >> cell._format;
    return stream;
}

void Softcan::Cell::clearData()
{
    _save_data = false;
    _x.clear();
    _x.shrink_to_fit();
    _y.clear();
    _y.shrink_to_fit();
}

QDataStream &operator>>(QDataStream &stream, Softcan &softcan)
{
    while (!stream.atEnd()) {
        softcan.add();
        stream >> softcan.cell();
        if (stream.atEnd()) {
            break;
        } else {
            unsigned char buf;
            stream >> buf;      //对齐位
            stream >> buf;      //对齐位
        }
    }
    return stream;
}

void Softcan::toCurve(Curve &curve)
{
    curve.clear();
    for (const auto &iter : _cells) {
        Curve::Cell cell(iter.index());
        cell.setDisplay(iter.visible());
        cell.setName(iter.name());
        cell.setUnit(iter.unit());
        cell.setWidth(iter.width());
        cell.setColor(iter.color());
        cell.setCanId(iter.canId());
        cell.setZeroByte(iter.zeroByte());
        cell.setHighByte(iter.highByte() - 2); //很重要，为什么要-2，这是试出来的
        cell.setHighRange(0, iter.firstBit());
        cell.setLowByte(iter.lowByte());
        cell.setLowRange(iter.thirdBit(), iter.secondBit());
        cell.setRangeIn(__min(iter.inputMin(), iter.inputMax()),
                        __max(iter.inputMin(), iter.inputMax()));
        cell.setRangeOut((int) __min(iter.yMin(), iter.yMax()),
                         (int) __max(iter.yMin(), iter.yMax()));
        cell.setPrecision(0);
        cell.setRemark(iter.intro());
        curve.append(qMove(cell));
    }
    curve.setInitialized(true);
}

void Softcan::toTribe(Tribe &tribe)
{
    tribe.clear();
    for (const auto &iter : _cells) {
        if (iter.canId() == 0) {
            break;      //导入时会出bug
        }
        Tribe::Style style;
        style.setIndex(iter.index());
        style.setDisplay(iter.visible());
        style.setName(iter.name());
        style.setUnit(iter.unit());
        style.setWidth(iter.width());
        style.setColor(iter.color());
        style.setRangeOut((int) __min(iter.yMin(), iter.yMax()),
                          (int) __max(iter.yMin(), iter.yMax()));
        style.setPrecision(0);
        style.setRemark(iter.intro());
        tribe.styles().append(qMove(style));
        Tribe::Cell cell(iter.name());
//        qDebug() << "Softcan::toTribe y.size() " << iter.y().size();
        if (iter.saveFlag()) {
            for (const auto &v : iter.y()) {
                cell.push(Tribe::Data, (float) v);
            }
        }
        tribe.cells().append(qMove(cell));
    }
    tribe.setLen();
}


QStringList Softcan::str() const
{
    QStringList list;
    for (const auto &iter : _cells) {
        list.append(iter.str());
    }
    return list;
}

bool Softcan::checkMagic(const char *array)
{
    //! \brief Softcan老版本魔术头
    const char *header("CCurveRecord");
#if 0   //测试用
    QString header_s;
    QString array_s;
    for (int i = 0; i < SOFTCAN_MAGIC_LEN; ++i) {
        header_s += QString("%1 ").arg(header[i],2,16,QChar('0'));
        array_s += QString("%1 ").arg(array[i],2,16,QChar('0'));
    }
    qDebug() << "headr: " << header_s;
    qDebug() << "array: " << array_s;
#endif
    // 第一个为校验码之类的，不是确定值，所以从1开始
    for (int i = 0; i < SOFTCAN_MAGIC_LEN; ++i) {
        if (array[i] != header[i]) {
            return false;
        }
    }
    return true;
}

void Softcan::clearDataSpace()
{
    for (auto &iter : _cells) {
        iter.clearData();
    }
}

