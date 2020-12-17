#include "../include/SuperPower.hpp"
#include <QGraphicsScene>
#include <QPainter>
#include <iostream>
#include <string.h>
#include <QDebug>
#include <QRandomGenerator>
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
    if(strcmp(m_type,"health")==0){
        painter->setBrush(Qt::white);
        painter->drawRect(0, 0, m_size, m_size);
        painter->setBrush(Qt::red);
        painter->drawRect(0, 10,m_size,10);
        painter->drawRect(10, 0,10,m_size);
    }
    else if (strcmp(m_type,"superpower")==0){
        painter->setBrush(Qt::yellow);
        painter->drawRect(0, 0, m_size, m_size);
        painter->setBrush(Qt::red);
        painter->drawRect(5, 0,21,7);
        painter->drawRect(5, 0,7,13);
        painter->drawRect(5, 13,21,7);
        painter->drawRect(18,13,7,17);
        painter->drawRect(5, 23,21,7);
    }
    else if (strcmp(m_type,"speed")==0){
        painter->setBrush(Qt::green);
        painter->drawRect(0, 0, m_size, m_size);
        painter->setBrush(Qt::white);
        painter->drawRect(0, 10,m_size,10);
        painter->drawRect(10, 0,10,m_size);
    }

    while (!scene()->collidingItems(this).isEmpty()) {
        QList<QGraphicsItem *> itemsss = scene()->collidingItems(this);
        for (auto item : itemsss) {
            //0 je id elementa Wall
            if (item->type() == 0 ) {
                m_x=QRandomGenerator::global()->bounded(1240);
                m_y=QRandomGenerator::global()->bounded(600);
                qDebug()<<"uso ovdeee";
                setPos(m_x,m_y);
            }
        }
    }

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

