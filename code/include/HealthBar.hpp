#ifndef HEALTHBAR_HPP
#define HEALTHBAR_HPP

#include <QObject>
#include <QGraphicsRectItem>

class HealthBar : public QObject, public QGraphicsRectItem
{
Q_OBJECT
public:
    HealthBar(float x, float y, int width, int height);
    HealthBar(int width, int height);
    ~HealthBar();

    QGraphicsRectItem* bar_frame;
    QGraphicsRectItem* bar;

    int width;
    int height;
    float m_x;
    float m_y;
};

#endif // HEALTHBAR_HPP
