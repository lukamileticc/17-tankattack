#include "../include/rocket_1.hpp"

Rocket::Rocket(float x, float y, float r, int rocket_power)
    :m_x(x),m_y(y),m_r(r)
{
    if(rocket_power == 0) easy_rocket = 1;
    else if(rocket_power == 1) medium_rocket = 1;
    else if(rocket_power == 2) hard_rocket = 1;
    else throw "Nepodrzana jacina metka";
}
QRectF Rocket::boundingRect() const
{
}
void Rocket::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    painter->setBrush(Qt::yellow);
    painter->drawEllipse(m_x,m_y,m_r,m_r);
}
