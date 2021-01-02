#ifndef ROCKET_HPP
#define ROCKET_HPP

#include "code/include/Input.hpp"
#include <QPainter>
#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include "Wall.hpp"
#include "Map.hpp"
#include "Tank.hpp"
#include <memory>
#include <utility>
#include <QObject>

class Rocket : public QObject , public QGraphicsItem {
Q_OBJECT
public:

    static int rakete_tenka_0;
    static int rakete_tenka_1;

    Rocket(float x,float y,float r ,const Rocket_type,int id, int x_v, int y_v, qreal rot,Tank *t);
    ~Rocket();

    // QGraphicsItem interface
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int type() const override;
    QPainterPath shape() const override;

public slots:
    void move();

private:

    //koordinate rakete i poluprecnik
    float m_x;
    float m_y;
    float m_r;
    Rocket_type m_rocket_type;

    int m_id;
    int m_direction_x;
    int m_direction_y;
    qreal m_rotation;
    float m_life_time = 0;

    QColor m_color;
    Tank *m_t;
    int m_rocket_power;

    QTimer *timer = nullptr;

    bool first = true;
    QPointF center;
};

#endif //ROCKET_HPP


