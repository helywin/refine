//
// Created by jiang.wenqiang on 2018/12/18.
//

#ifndef REFINE_COMMUNICATE_HPP
#define REFINE_COMMUNICATE_HPP

#include <QtCore/QThread>
#include "Message.hpp"

class RecvBuffer;

class Communicate : public QThread, public Message
{
Q_OBJECT
public:
    enum Type
    {
        None,
        Command,
        Burning
    };

    class Cell
    {
    private:
        Type _type;
        QByteArray _data;
    public:
        Cell(const Cell &cell) = default;

        Cell(Type type, const QByteArray &bytes) : _type(type), _data(bytes) {}

        Cell(Type type, QByteArray &&bytes) : _type(type), _data(bytes) {}

        inline Type type() const { return _type; }

        inline const QByteArray &data() const { return _data; }
    };
private:
    RecvBuffer *_buffer;
    QList<Cell> _sequence;

public:
    explicit Communicate(Message *message = nullptr);
    void setParams(RecvBuffer *buffer);
    void burnProgram(QByteArray &&bytes);
    void sendCommand(QByteArray &&command);

    void begin() {}
    void stop() {}

protected:
    void run() override;
};


#endif //REFINE_COMMUNICATE_HPP
