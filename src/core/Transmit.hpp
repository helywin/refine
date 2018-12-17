/*******************************************************************************
 * @file
 * @author jiang.wenqiang
 * @date 2018/10/16
 * @brief 发送报文
 * @details 模拟采集时接收的报文，发送到CAN总线上
 ******************************************************************************/

#ifndef REFINE_TRANSMIT_HPP
#define REFINE_TRANSMIT_HPP

#include <QtCore/QThread>
#include "Message.hpp"

class Buffer;
class Can;

class Transmit : public QThread, public Message
{
Q_OBJECT
public:
    enum ResponseType
    {
        Send,
        Receive
    };
    Q_ENUMS(ResponseType)

    enum TransmitType
    {
        None,
        Command,
        Burning
    };

    class Cell
    {
    private:
        TransmitType _type;
        QByteArray _data;
    public:
        Cell(const Cell &cell) = default;

        Cell(TransmitType type, const QByteArray &bytes) : _type(type), _data(bytes) {}

        Cell(TransmitType type, QByteArray &&bytes) : _type(type), _data(bytes) {}

        inline TransmitType type() const { return _type; }

        inline const QByteArray &data() const { return _data; }
    };

private:
    Buffer *_buffer;
    Can *_can;
    QList<Cell> _sequence;

public:
    explicit Transmit(Message *message = nullptr);
    void setParams(Can *can, Buffer *buffer);
    void burnProgram(QByteArray &&bytes);
    void sendCommand(QByteArray &&command);

protected:
    void run() override;

signals:
    void response(ResponseType type, const QString &response);
    void progress(double value);

};


#endif //REFINE_TRANSMIT_HPP
