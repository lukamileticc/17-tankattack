#ifndef ROCKET_HPP
#define ROCKET_HPP

#include <QPainter>
#include <memory>
#include <utility>
#include "Map.hpp"
#include "Tank.hpp"
#include <QObject>
#include <QGraphicsItem>
#include "code/include/Input.hpp"

class Rocket : public QObject , public QGraphicsItem {
    Q_OBJECT
public:
    Rocket(float x,float y,float r ,int rocket_power,Input* input);
    ~Rocket() = default;

    // QGraphicsItem interface
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
    void move();

private:

    float m_life_time = 0;
    //koordinate rakete i poluprecnik
    float m_x;
    float m_y;
    float m_r;
    //boja rakete ce se menjati u zavisnosti od jacine koju tenk poseduje
    QColor m_boja;

};
#endif //ROCKET_HPP


