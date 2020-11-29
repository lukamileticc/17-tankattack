#include "code/include/Tank.hpp"
#include "../include/rocket_1.hpp"

#include <QObject>>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>


Tank::Tank(QColor color, float x, float y)
    :m_color(color), m_x(x), m_y(y)
{
}

void Tank::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(m_color);

    painter->drawRect(m_x, m_y, 30, 30);
}

QRectF Tank::boundingRect() const
{
    return QRectF(m_x, m_y, 30, 30);
}
float Tank::getXposition() const{
    return m_x;
}
float Tank::getYposition() const{
    return m_y;
}
void Tank::keyPressEvent(QKeyEvent *event){
//    qDebug() << "Tenk reaguje da je kliknuto na njega";

    if(event->key() == Qt::Key_Space){
        //pravimo metak
        float rocket_x_position = getXposition() + boundingRect().width()/2;
        float rocket_y_position = getYposition() + boundingRect().width()/2;
        float diff = boundingRect().width()/4;
        Rocket *rocket = new Rocket(rocket_x_position+diff,rocket_y_position-diff,15,2);
        rocket->setPos(rocket->x(),rocket->y());
        qDebug() << "Rocket je napravljen!";
        scene()->addItem(rocket);
        qDebug() << scene()->items().size();
//        scene()->update();
    }
}
