#include "../include/Wall.hpp"
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>


Wall::Wall(float x, float y, float height, float width)
    :m_x(x), m_y(y), m_height(height), m_width(width)
{
}

Wall::Wall(const Wall&)
{

}
Wall Wall::operator=(const Wall&)
{

}

void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::white);

    painter->drawRect(m_x, m_y, m_width, m_height);
}

QRectF Wall::boundingRect() const
{
    return QRectF(m_x, m_y, 10 + m_width, 10 + m_height);
}
