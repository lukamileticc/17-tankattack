#ifndef TANK_HPP
#define TANK_HPP


#include <vector>
//#include "Rocket.hpp"

class Tank {
public:
//    Tank(int x,int y,int speed,std::vector<Rocket> rockets);
    ~Tank()  = default;
    bool IsAbleToShoot() const;

    void DecreaseHealth(int health);
    void IncreaseHealth(int health);

    float GetCurrentHealth() const;
    bool IsDead() const;
    int GetCurrentNumsOfLife () const;
    int GetX() const;
    int GetY() const;
    int GetSpeed() const;
    int GetCurrentNumsOfRockets() const;
    int GetScore() const;
//    const std::vector<Rocket> &getRockets() const;

//    void setRockets(const std::vector<Rocket> &mRockets);
    void SetX(int x);
    void SetY(int y);
    void SetSpeed(int speed);
    void SetCanShot();
    void DecreaseNumOfLife();
    void IncreaseNumOfLife();
    void shoot();
    void IncreaseScore(int score);


private:
    int m_x;// x koordinata polozaja
    int m_y; // y koordinata polozaja
    int m_speed; // brzina tenka
    float m_current_health; //trenutno helti
    int m_num_of_lives; //broj preostalih zivota
    int m_num_of_rockets; // broj raketa
//    std::vector<Rocket> m_rockets; //niz raketa
    bool m_can_shoot;
    int m_score;

};
#endif //TANK_HPP
