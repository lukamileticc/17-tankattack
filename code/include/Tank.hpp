#ifndef TANK_HPP
#define TANK_HPP
#include <QGraphicsItem>
#include <vector>
#include <QObject>
//#include "Rocket.hpp"

class Tank : public QGraphicsItem {

public:
//    Tank(int x,int y,int speed,std::vector<Rocket> rockets);

    Tank(QColor color, float x, float y);

    ~Tank()  = default;

    // QGraphicsItem interface
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool IsAbleToShoot() const;


    void keyPressEvent(QKeyEvent *event);

    float getXposition() const;
    float getYposition() const;

private:
    float m_x;// x koordinata polozaja
    float m_y; // y koordinata polozaja
    QColor m_color;

};
#endif //TANK_HPP
