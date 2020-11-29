#include "code/include/rocket_1.hpp"
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

Rocket::Rocket(float x, float y, float r, int rocket_power)
    :m_x(x),m_y(y),m_r(r)
{
    if(rocket_power == 0) easy_rocket = 1;
    else if(rocket_power == 1) medium_rocket = 1;
    else if(rocket_power == 2) hard_rocket = 1;
    else throw "Nepodrzana jacina metka";

    //connect
    QTimer *timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));

    //na svakih 50ms ce se slati signal i onda ce se pozivati move;
    timer->start(50);


}
void Rocket::move()
{
    setPos(x() + 10, y());
//    qDebug() << "Raketa se pomera svakih 50 ms!";
    if(pos().x() > 1280)
    {
        scene()->removeItem(this);
        delete this;
        qDebug() << "Raketa se obrisala";
    }

}
QRectF Rocket::boundingRect() const
{
    return QRectF(m_x-m_r,m_y-m_r,2*m_r,2*m_r);
}
void Rocket::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    painter->setBrush(Qt::yellow);
    painter->drawEllipse(m_x,m_y,m_r,m_r);
}
