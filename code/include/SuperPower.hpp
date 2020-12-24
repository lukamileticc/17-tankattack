#ifndef SUPERPOWER_HPP
#define SUPERPOWER_HPP

#include <QGraphicsItem>



class SuperPower : public QGraphicsItem {
public:

    SuperPower() = default;
    SuperPower(QString type,float x,float y,float size);
    ~SuperPower()  = default;

    QString getType() const;
    float getSize() const;
    void setSize(float size);

    void setType(char * type);
    int type() const;

    // QGraphicsItem interface
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QString m_type;
    float m_x;
    float m_y;
    float m_size;
    bool m_is_positined = false;

};

#endif // SUPERPOWER_HPP
