#ifndef TANK_HPP
#define TANK_HPP
#include <QGraphicsItem>
#include "code/include/Input.hpp"
#include "HealthBar.hpp"
#include "Client.hpp"
#include <QMediaPlayer>
#include <vector>

class Tank : public QObject, public QGraphicsItem{
    Q_OBJECT
public:

    Tank(int id,QColor color, float x, float y, Input *input, QColor host, QColor client, bool isMultiPlayer);

    ~Tank();

    // QGraphicsItem interface
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;
    int type() const override;

    bool IsAbleToShoot() const;

    void decrease_health(int health);
    void increase_health(int health);

    void show_tank_info();

    QMediaPlayer *get_explosion_sound() const;
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
    int getId();
    void SetX(float x);
    void SetY(float y);
    void SetSpeed(float speed);
    void SetCanShot();
    void DecreaseNumOfLife();
    void IncreaseNumOfLife();
    void shoot();
    void IncreaseScore(int score);
    void SetHealth(float health);
    QString get_name() const;
    void set_name(const QString &name);
    //---
    void destroy();

    float getXposition() const;
    float getYposition() const;

    bool is_pause();
    bool is_destroyed();
    void set_end_of_round();

    void move_forward();
    void move_backward();
    void rotate(float angle);

    void setColors();

    HealthBar* m_healt_bar;
    HealthBar* m_health_bar_tank;

public slots:
    void advance();

private:
    //zvuk rakete
    QMediaPlayer *rocket_sound = nullptr;
    QMediaPlayer *tank_hit = nullptr;

    int m_id;
    QColor m_color;
    float m_x;// x koordinata polozaja
    float m_y; // y koordinata polozaja
    float m_speed = 0;
    float m_power = 0;// brzina tenka
    float m_current_health; //trenutno helti
    int m_num_of_lives; //broj preostalih zivota
    int m_num_of_rockets; // broj raketa

    bool m_can_shoot;
    int m_score = 0;
    int m_health = 100;
    bool m_end_of_round = false;
    QString m_name;

    Rocket_type m_tank_rocket_type = Rocket_type::Low_power;

    Input *m_input;
    float x_v = 0;
    float y_v = 1;

    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool launch = false;
    bool m_pause = false;

    bool destroyed = false;

    //Deo koji se odnosi na rad mreze
    //~~~~~~~~~~~~~
    bool m_isMultiPlayer = false;
    Client *m_Client;
    QColor m_HostColor = Qt::blue;
    QColor m_ClientColor = Qt::red;
    //~~~~~~~~~~~~~
};

#endif //TANK_HPP
