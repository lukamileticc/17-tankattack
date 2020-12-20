#ifndef TANK_HPP
#define TANK_HPP
#include <QGraphicsItem>
#include "code/include/Input.hpp"
#include <vector>
//#include "Rocket.hpp"

class Tank : public QObject, public Input{
    Q_OBJECT
public:
//    Tank(int x,int y,int speed,std::vector<Rocket> rockets);

    Tank(int id,QColor color, float x, float y, Input *input);

    ~Tank()  = default;

    int type() const;

    // QGraphicsItem interface

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    bool IsAbleToShoot() const;

    void decrease_health(int health);
    void increase_health(int health);

    void show_tank_info();

    int get_current_health() const;
    bool IsDead() const;
    int GetCurrentNumsOfLife () const;
    float GetX() const;
    float GetY() const;
    float GetSpeed() const;
    int GetCurrentNumsOfRockets() const;

    int get_score() const;
    void set_score(int score);
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

    QString get_name() const;
    void set_name(const QString &name);
    //---
    void destroy();

    float getXposition() const;
    float getYposition() const;

    bool is_pause();
    bool is_destroyed();
    void set_end_of_round();

public slots:
    void advance();
    //void keyPressEvent(QKeyEvent *event) override;
    //void keyReleaseEvent(QKeyEvent *event) override;
    //---

private:
    int m_id;
    QColor m_color;
    float m_x;// x koordinata polozaja
    float m_y; // y koordinata polozaja
    float m_speed; // brzina tenka
    int m_num_of_lives; //broj preostalih zivota
    int m_num_of_rockets; // broj raketa
//    std::vector<Rocket> m_rockets; //niz raketa
    bool m_can_shoot;
    int m_score = 0;
    int m_health = 100;
    bool m_end_of_round = false;
    QString m_name;

    Rocket_type m_tank_rocket_type = Rocket_type::Low_power;

    bool destroyed = false;
    Input *m_input;
    //~~~~~~~~~~~~~
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    /////////////////////
    bool launch = false;
    bool m_pause = false;


    float x_v = 0;
    float y_v = 1;

    float rotation_angle = 0.5;
    //~~~~~~~~~~~~~
};
#endif //TANK_HPP
