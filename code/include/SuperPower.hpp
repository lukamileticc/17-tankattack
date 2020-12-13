#ifndef SUPERPOWER_HPP
#define SUPERPOWER_HPP

#include <QGraphicsItem>



class SuperPower : public QGraphicsItem {
public:

    SuperPower() = default;
    SuperPower(char * type,float x,float y,float size);
    ~SuperPower()  = default;
    char * getType() const;

    void setType(char * type);

private:
    char * m_type;
    float m_x;
    float m_y;
    float m_size;

    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // SUPERPOWER_HPP
