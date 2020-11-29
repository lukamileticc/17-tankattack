#ifndef ROCKET_1_HPP
#define ROCKET_1_HPP

//trebam napraviti enum u kome ce biti tri jacine metaka
//1. skida 10p,2. skida 20p i 3. jacina metka skida 30p

#include <QObject>
#include <QPainter>
#include <QGraphicsRectItem>

class Rocket : public QObject,public QGraphicsItem{
    Q_OBJECT
public:
    Rocket(float x,float y,float m_r,int rocket_power);
    ~Rocket() = default;

    // QGraphicsItem interface
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    float getX() const;
    float getY() const;

public slots:
    void move();

private:

    //falgovi za jacinu metka koji se pokupio
    int easy_rocket = 0;
    int medium_rocket = 0;
    int hard_rocket = 0;

    //koordinate rakete
    float m_x;
    float m_y;
    float m_r;
};

#endif // ROCKET_1_HPP
