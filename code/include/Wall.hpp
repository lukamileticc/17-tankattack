#ifndef WALL_HPP
#define WALL_HPP

#include <QGraphicsItem>

class Wall : public QGraphicsItem
{
public:
    //prima x,y koordinate gornjeg levog ugla pravougaonika
    //njegovu visinu i sirinu
    Wall(float x, float y, float width, float height);

    ~Wall() = default;

    int type() const;

    Wall(const Wall&);
    Wall operator=(const Wall&);

    // QGraphicsItem interface
    //boundingRect() definise pravougaonik u kome Qt crta objekat
    QRectF boundingRect() const;

    //metod koji sluzi za crtanje
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool isVertical() const;
    bool isHorizontal() const;

    //vraca par koordinata gornjeg levog ugla pravougaonika
    std::pair<float,float> getCoordinates() const;

    float getWidth() const;
    float getHeight() const;

private:
    //koordinate gornjeg levog ugla pravougaonika (x,y)
    float m_x;
    float m_y;

    //visina i sirina pravougaonika
    float m_height;
    float m_width;
};

#endif //WALL_HPP
