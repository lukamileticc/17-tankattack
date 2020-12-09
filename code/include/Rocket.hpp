#ifndef ROCKET_HPP
#define ROCKET_HPP

#include <QPainter>
#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <memory>
#include <utility>
#include "Wall.hpp"
#include "Map.hpp"
#include "Tank.hpp"
#include "code/include/Input.hpp"


class Rocket : public QObject , public QGraphicsItem {
    Q_OBJECT
public:

    static int rakete_tenka_0;
    static int rakete_tenka_1;

    Rocket(float x,float y,float r ,const Rocket_type,int id, int x_v, int y_v, qreal rot);
    ~Rocket() = default;

    // QGraphicsItem interface
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    int type() const override;

public slots:
    void move();

private:

    //koordinate rakete i poluprecnik
    float m_x;
    float m_y;
    float m_r;
    //da vidimo kom tenku pripada raketa
    int m_id;
    int m_pravac_x;
    int m_pravac_y;
    qreal m_rotation;
    float m_life_time = 0;
    //boja rakete ce se menjati u zavisnosti od jacine koju tenk poseduje
    QColor m_boja;

    Rocket_type m_rocket_type;
    int m_rocket_power;

};
#endif //ROCKET_HPP


