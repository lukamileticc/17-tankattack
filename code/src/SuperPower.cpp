#include "../include/SuperPower.hpp"
#include <QGraphicsScene>
#include <QPainter>
#include <iostream>
#include <string.h>
#include <QDebug>
#include <QRandomGenerator>

SuperPower::SuperPower(QString type,float x,float y,float size)
    : m_type(type),m_x(x),m_y(y),m_size(size)
{
    setPos(m_x, m_y);
}
QString SuperPower::getType() const {
    return m_type;
}

float SuperPower::getSize() const {
    return m_size;
}

void SuperPower::setSize(float size) {
    m_size=size;
}

void SuperPower::setType(char *type) {
    m_type=type;
}

int SuperPower::type() const {
    return 4;
}

QRectF SuperPower::boundingRect() const {
    return QRectF(0, 0, m_size, m_size);
}


void SuperPower::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(m_type == QString("health")) {
        QPixmap pixmap=QPixmap(":/resources/images/health.png");
        pixmap.scaled(m_size,m_size);
        painter->setBrush(pixmap);
        painter->setPen(Qt::NoPen);
        painter->drawRect(0, 0, m_size, m_size);
    }
    else if (m_type == QString("superpower")) {
        QPixmap pixmap=QPixmap(":/resources/images/boost.png");
        pixmap.scaled(m_size,m_size);
        painter->setPen(Qt::NoPen);
        painter->setBrush(pixmap);
        painter->drawRect(0, 0, m_size, m_size);
    }

    //ovo parce koda dovodi do greske kad raketa ima koliziju sa objektom ove klase
    if(!m_is_positined) {
        while(!scene()->collidingItems(this).isEmpty()){
            m_x=QRandomGenerator::global()->bounded(1240);
            m_y=QRandomGenerator::global()->bounded(600);
            qDebug()<<"uso ovdeee";
            setPos(m_x,m_y);
        }
        m_is_positined = true;
    }

}

//void SuperPower::advance() {
//    QList<QGraphicsItem *> items = scene()->collidingItems(this);

//    for (auto item : items) {
//        //0 je id elementa Wall
//        if (item->type() == 1) {
//            std::cout<<"usaoo";
//        }
//    }
//}

