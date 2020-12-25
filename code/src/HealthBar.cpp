#include "../include/HealthBar.hpp"

#include <QPainter>
#include <iostream>

HealthBar::HealthBar(float x,float y,int width, int height)
    : width(width), height(height),m_x(x),m_y(y)
{
    barFrame = new QGraphicsRectItem;
    barFrame->setRect(x,y, width, height);
    barFrame->setBrush(Qt::gray);

    bar = new QGraphicsRectItem(x,y, width, height);
    bar->setBrush(Qt::green);
}

HealthBar::HealthBar(int width, int height)
{
    barFrame = new QGraphicsRectItem;
    barFrame->setRect(x(),y(), width, height);
    barFrame->setBrush(Qt::gray);

    bar = new QGraphicsRectItem(x(),y(), width, height);
    bar->setBrush(Qt::green);
}

HealthBar::~HealthBar()
{
    delete this->bar;
    delete this->barFrame;
}
