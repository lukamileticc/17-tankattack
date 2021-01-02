#include "../include/HealthBar.hpp"
#include <QPainter>
#include <iostream>

HealthBar::HealthBar(float x,float y,int width, int height)
    : width(width), height(height),m_x(x),m_y(y)
{
    bar_frame = new QGraphicsRectItem;
    bar_frame->setRect(x,y, width, height);
    bar_frame->setBrush(Qt::gray);

    bar = new QGraphicsRectItem(x,y, width, height);
    bar->setBrush(Qt::red);
}

//HealthBar::HealthBar(int width, int height) {
//    bar_frame = new QGraphicsRectItem;
//    bar_frame->setRect(x(),y(), width, height);
//    bar_frame->setBrush(Qt::gray);

//    bar = new QGraphicsRectItem(x(),y(), width, height);
//    bar->setBrush(Qt::red);
//}

HealthBar::~HealthBar() {
    delete this->bar;
    delete this->bar_frame;
}
