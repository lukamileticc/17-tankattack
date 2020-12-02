#include "../include/Wall.hpp"
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>
#include<QGraphicsView>

Wall::Wall(float x, float y, float height, float width)
    :m_x(x), m_y(y), m_width(width), m_height(height)
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

    painter->drawRect(m_x, m_y - 45, m_height, m_width);

}

QRectF Wall::boundingRect() const
{
    return QRectF(m_x, m_y - 45, m_width, m_height);
}

bool Wall::isVertical() const
{
    return (m_height - m_width) > 0 ? true : false;
}

bool Wall::isHorizontal() const
{
    return !(Wall::isVertical()) ? true : false;
}

float Wall::getHeight() const
{
    return m_height;
}

float Wall::getWidth() const
{
    return m_width;
}

std::pair<float,float> Wall::getCoordinates() const
{
    return std::make_pair(m_x, m_y);
}



