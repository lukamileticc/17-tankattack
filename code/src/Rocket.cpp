#include "code/include/Rocket.hpp"
#include <QDebug>
#include <cmath>

int Rocket::rakete_tenka_0 = 0;
int Rocket::rakete_tenka_1 = 0;

//u rocket power ce se prosledjivati enum u zavisnoti koju jacinu poseduje tenk
Rocket::Rocket(float x, float y, float r, int rocket_power,Input* input,int id, int x_v, int y_v, qreal rot)
    :m_x(x),m_y(y),m_r(r),m_id(id),m_pravac_x(x_v),m_pravac_y(y_v),m_rotation(rot)
{

    if(id == 0) //prvi tenk pravi raketu
        rakete_tenka_0 += 1;
    else //drugi tenk pravi raketu
        rakete_tenka_1 += 1;


    if(rocket_power == 0) m_boja = Qt::red;
    else if(rocket_power == 1) m_boja = Qt::yellow;
    else if(rocket_power == 2) m_boja = Qt::blue;
    else throw "Nepodrzana jacina metka";


    connect(input->timer,SIGNAL(timeout()),this,SLOT(move()));

}
void Rocket::move()
{
    setPos(x() - m_pravac_x,y() - m_pravac_y);
//    if (!scene()->collidingItems(this).isEmpty()) {
//    }

    m_life_time++;
    if(m_life_time > 150)
    {
        if(m_id == 0)
            rakete_tenka_0 -= 1;
        else
            rakete_tenka_1 -= 1;

        scene()->removeItem(this);
        delete this;
        qDebug() << "Raketa je unistena: " << rakete_tenka_0;
    }
}
QRectF Rocket::boundingRect() const
{
    //prodiskutovati o ovome
    return QRectF(m_x-m_r,m_y-m_r,2*m_r,2*m_r);
}
void Rocket::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(m_boja);
    painter->drawEllipse(m_x,m_y,m_r,m_r);
}
