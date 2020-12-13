#include "../include/SuperPower.hpp"
#include <QGraphicsScene>
#include <QPainter>
#include <iostream>
#include <string.h>
SuperPower::SuperPower(char *type,float x,float y,float size)
    : m_type(type),m_x(x),m_y(y),m_size(size)
{
    setPos(m_x, m_y);
}
char * SuperPower::getType() const
{
    return m_type;
}

void SuperPower::setType(char *type)
{
    m_type=type;
}

int SuperPower::type() const
{
    return 4;
}

QRectF SuperPower::boundingRect() const
{
    return QRectF(0, 0, m_size, m_size);
}

void SuperPower::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(strcmp(m_type,"health")==0)
        painter->setBrush(Qt::red);
    else if (strcmp(m_type,"superpower")==0)
        painter->setBrush(Qt::yellow);
    painter->drawRect(0, 0, m_size, m_size);

}

//void SuperPower::advance()
//{
//    QList<QGraphicsItem *> items = scene()->collidingItems(this);

//    for (auto item : items) {
//        //0 je id elementa Wall
//        if (item->type() == 1) {
//            std::cout<<"usaoo";
//        }
//}
//    }

