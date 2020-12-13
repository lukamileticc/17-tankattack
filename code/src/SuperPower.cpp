#include "../include/SuperPower.hpp"

#include <QPainter>
SuperPower::SuperPower(char *type,float x,float y,float size)
    : m_type(type),m_x(x),m_y(y),m_size(size)
{
    setPos(m_x, m_y);
}
char * SuperPower::getType() const
{
    return m_type;
}

void SuperPower::setType(char *type)
{
    m_type=type;
}

QRectF SuperPower::boundingRect() const
{
    return QRectF(0, 0, m_size, m_size);
}

void SuperPower::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::red);

    painter->drawRect(0, 0, m_size, m_size);
}

