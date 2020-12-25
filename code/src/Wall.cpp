#include "../include/Wall.hpp"
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>
#include<QGraphicsView>

Wall::Wall(float x, float y, float width, float height)
    :m_x(x), m_y(y), m_height(height), m_width(width)
{
    setPos(m_x, m_y);
}

Wall::Wall(const Wall&)
{
}

Wall Wall::operator=(const Wall&)
{
    return *this;
}

void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(QColor(180, 165, 208));
    painter->setPen(Qt::NoPen);
    painter->drawRect(0, 0, m_width, m_height);
}

QRectF Wall::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
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
    return !isVertical();
}

float Wall::getHeight() const
{
    return m_height;
}

float Wall::getWidth() const
{
    return m_width;
}

float Wall::getX() const{
    return m_x;
}

float Wall::getY() const{
    return m_y;
}

std::pair<float,float> Wall::getCoordinates() const
{
    return std::make_pair(m_x, m_y);
}
