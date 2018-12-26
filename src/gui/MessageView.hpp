//
// Created by jiang.wenqiang on 2018/12/26.
//

#ifndef REFINE_MESSAGEVIEW_HPP
#define REFINE_MESSAGEVIEW_HPP

#include <QtWidgets/QListView>

class MessageView : public QListView
{
Q_OBJECT
private:
public:
    MessageView(QWidget *parent = nullptr);
};


#endif //REFINE_MESSAGEVIEW_HPP
