#include "../include/Wall.hpp"
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>
#include<QGraphicsView>
#define UNUSED(x) (void)(x)

Wall::Wall(float x, float y, float height, float width)
    :m_x(x), m_y(y), m_height(height), m_width(width)
{
}

Wall::Wall(const Wall&)
{

}

Wall Wall::operator=(const Wall&)
{
//    return *this;
}

void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    UNUSED(option);
    UNUSED(widget);

    painter->setBrush(Qt::white);

    painter->drawRect(m_x, m_y - 45, m_height, m_width);

}

QRectF Wall::boundingRect() const
{
    return QRectF(m_x, m_y - 45, m_height, m_width);
}

int Wall::type() const {
    return 0;
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
