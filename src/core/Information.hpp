//
// Created by jiang.wenqiang on 2018/12/26.
//

#ifndef REFINE_INFORMATION_HPP
#define REFINE_INFORMATION_HPP

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QVector>
#include <QtCore/QThread>
#include <QtCore/QTextStream>
#include "Global.hpp"

class Information : public QThread
{
Q_OBJECT
public:
    class Cell
    {
    private:
        Re::MessageTypes _type;
        QString _message;
        QDateTime _time;
    public:
        Cell(Re::MessageTypes type, const QString &msg);

        inline Re::MessageTypes type() const
        { return _type; }

        inline const QString &message() const
        { return _message; }

        inline QDateTime time() const
        { return _time; }

        QString str() const;
    };

private:
    QList<Cell> _all_infos;
    QVector<int> _info_lists;
    QVector<int> _warning_lists;
    QVector<int> _critical_lists;
    QVector<int> _fatal_lists;
    QVector<int> _debug_lists;
    QVector<int> _can_lists;
    QTextStream *_stream;
    int _index = 0;
    bool _is_clear = false;

public:
    Information();

    ~Information() final;

    void clear(Re::MessageTypes type);

    inline int size() const
    { _all_infos.size(); }

    int size(Re::MessageTypes type) const;

    inline const Cell &data(int index) const
    {
        Q_ASSERT(index >= 0 && index < _all_infos.size());
        return _all_infos[index];
    }

    const Cell &data(int index, Re::MessageTypes type) const;

    const QVector<int> &indexList(Re::MessageTypes type) const;

public slots:
    void getMessage(Re::MessageTypes type, const QString &msg);

protected:
    void run() override;
};


#endif //REFINE_INFORMATION_HPP
