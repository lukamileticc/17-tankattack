#include "code/include/Rocket.hpp"

//u rocket power ce se prosledjivati enum u zavisnoti koju jacinu poseduje tenk
Rocket::Rocket(float x, float y, float r, int rocket_power,Input* input)
    :m_x(x),m_y(y),m_r(r)
{

    if(rocket_power == 0) m_boja = Qt::red;
    else if(rocket_power == 1) m_boja = Qt::yellow;
    else if(rocket_power == 2) m_boja = Qt::blue;
    else throw "Nepodrzana jacina metka";


    connect(input->timer,SIGNAL(timeout()),this,SLOT(move()));

}
void Rocket::move()
{
    if(m_life_time > 150)
    {
        scene()->removeItem(this);
        delete this;
    }
    setPos(x() + 10,y());



    m_life_time++;
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
