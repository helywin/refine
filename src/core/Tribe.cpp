/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/9/11
 * @brief 曲线数据缓冲区
 * @details 存储曲线数据缓冲区
 ******************************************************************************/

#include <QtCore/QDataStream>
#include "Tribe.hpp"
#include "Csv.hpp"

float Tribe::Cell::fakePercent() const
{
    int cnt = 0;
    for (const auto iter : _fill) {
        if (iter != Data) {
            cnt += 1;
        }
    }
    return (float) cnt / (float) _fill.size();
}

void Tribe::Cell::push(const Tribe::FillType fill, const float &v)
{
    _fill.append(fill);
    _data.push_back(v);
    _fill_this = true;
    if (fill == Data) {
        _empty = false;
    }
}

Tribe::Tribe(Message *message) :
        Message(message), _len(0), _msec(10) {}

/*!
 * @brief 清空数据
 */
void Tribe::Cell::reset()
{
    _data.clear();
    _fill.clear();
    _fill_this = false;
    _empty = true;
}

bool Tribe::dumpToCsv(QFile &f) const
{
    if (f.isOpen()) {
        f.close();
    }
    Csv csv;
    csv.setFile(&f);
    if (!csv.startWrite("gbk")) {
        qCritical("文件打开失败");
        return false;
    }
    csv.writeLine(_header);
    QStringList list;
    for (int i = 0; i < len(); ++i) {
        list.clear();
        for (const auto &iter : _cells) {
            list.append(QString::number(iter.data(i)));
        }
        csv.writeLine(list);
    }
    return csv.finishWrite();
}

/*!
 * @brief 清空数据不清空曲线
 */
void Tribe::reset()
{
    for (auto &iter : _cells) {
        iter.reset();
    }
    _len = 0;
    _segment.clear();
    _marks.clear();
}

void Tribe::setUnFilled()
{
    for (auto &cell : _cells) {
        cell._fill_this = false;
    }
}

Tribe::Style::Style() {}

Tribe::Style::Style(const Curve::Cell &cell)
{
    *this = cell;
}

Tribe::Style &Tribe::Style::operator=(const Curve::Cell &cell)
{
    _index = cell._index;
    _display = cell._display;
    _name = cell._name;
    _unit = cell._unit;
    _width = cell._width;
    _color = cell._color;
    _range_out[0] = cell._range_out[0];
    _range_out[1] = cell._range_out[1];
    _precision = cell._precision;
    _remark = cell._remark;
    return *this;
}

void Tribe::genFromCurve(const Curve &curve)
{
    clear();
    for (const auto &cell : curve) {
        _styles.append(Style(cell));
        _cells.append(Cell(cell.name(), Tribe::RawData));
        _header.append(cell.name());
    }
}

QStringList Tribe::header(Tribe::Selection selection) const
{
    QStringList list;
    switch (selection) {
        case SelectAll:
            return _header;
        case SelectChecked:
            for (const auto &iter : _styles) {
                if (iter.display()) {
                    list.append(iter.name());
                }
            }
            return list;
        case SelectUnchecked:
            for (const auto &iter : _styles) {
                if (!iter.display()) {
                    list.append(iter.name());
                }
            }
            return list;
        default:
            return list;
    }
}

void Tribe::displayAll()
{
    for (auto &iter : _styles) {
        iter.setDisplay(true);
    }
}

void Tribe::displayNone()
{
    for (auto &iter : _styles) {
        iter.setDisplay(false);
    }
}