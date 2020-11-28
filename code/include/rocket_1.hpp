#ifndef ROCKET_1_HPP
#define ROCKET_1_HPP

//trebam napraviti enum u kome ce biti tri jacine metaka
//1. skida 10p,2. skida 20p i 3. jacina metka skida 30p

#include <QObject>
#include <QPainter>
#include <QGraphicsItem>

class Rocket : public QGraphicsItem{

public:
    Rocket(float x,float y,float r,int rocket_power);
    ~Rocket() = default;

    // QGraphicsItem interface
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:

    //falgovi za jacinu metka koji se pokupio
    int easy_rocket = 0;
    int medium_rocket = 0;
    int hard_rocket = 0;

    //koordinate centra
    float m_x;
    float m_y;
    //poluprecnik
    float m_r;

};

#endif // ROCKET_1_HPP
