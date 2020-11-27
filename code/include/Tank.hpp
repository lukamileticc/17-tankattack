#ifndef TANK_HPP
#define TANK_HPP
#include <QGraphicsItem>
#include <vector>
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


    void DecreaseHealth(int health);
    void IncreaseHealth(int health);

    float GetCurrentHealth() const;
    bool IsDead() const;
    int GetCurrentNumsOfLife () const;
    float GetX() const;
    float GetY() const;
    float GetSpeed() const;
    int GetCurrentNumsOfRockets() const;
    int GetScore() const;
//    const std::vector<Rocket> &getRockets() const;

//    void setRockets(const std::vector<Rocket> &mRockets);
    void SetX(float x);
    void SetY(float y);
    void SetSpeed(float speed);
    void SetCanShot();
    void DecreaseNumOfLife();
    void IncreaseNumOfLife();
    void shoot();
    void IncreaseScore(int score);

    //---
    void advance(int step) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    //---

private:

    QColor m_color;
    float m_x;// x koordinata polozaja
    float m_y; // y koordinata polozaja
    float m_speed; // brzina tenka
    float m_current_health; //trenutno helti
    int m_num_of_lives; //broj preostalih zivota
    int m_num_of_rockets; // broj raketa
//    std::vector<Rocket> m_rockets; //niz raketa
    bool m_can_shoot;
    int m_score;

    //~~~~~~~~~~~~~
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    float x_v = 0;
    float y_v = 1;

    float rotation_angle = 0.5;
    //~~~~~~~~~~~~~
};
#endif //TANK_HPP
