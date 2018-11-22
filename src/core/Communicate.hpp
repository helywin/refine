//
// Created by jiang.wenqiang on 2018/11/22.
//

#ifndef REFINE_COMMUNICATE_HPP
#define REFINE_COMMUNICATE_HPP

#include <QtCore/QThread>
#include "Message.hpp"

class Communicate : public QThread, public Message
{
Q_OBJECT
private:
public:
    explicit Communicate(Message *message = nullptr);
};


#endif //REFINE_COMMUNICATE_HPP
