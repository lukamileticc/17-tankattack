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

    static int rakete_tenka_0;
    static int rakete_tenka_1;

    Rocket(float x,float y,float r ,int rocket_power,Input* input,int id, int x_v, int y_v, qreal rot);
    ~Rocket() = default;

    // QGraphicsItem interface
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
    void move();

private:
    qreal m_rotation;
    int m_pravac_x;
    int m_pravac_y;
    //da vidimo kom tenku pripada raketa
    int m_id;
    float m_life_time = 0;
    //koordinate rakete i poluprecnik
    float m_x;
    float m_y;
    float m_r;
    //boja rakete ce se menjati u zavisnosti od jacine koju tenk poseduje
    QColor m_boja;

};
#endif //ROCKET_HPP


