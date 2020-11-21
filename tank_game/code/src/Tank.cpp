#include "../include/Tank.hpp"

//Tank::Tank(int x,int y,int speed,std::vector<Rocket> rockets):
//        m_x(x), m_y(y), m_current_health(100), m_num_of_lives(5),
//        m_num_of_rockets(10),
//        m_rockets(std::move(rockets)),
//        m_speed(speed),m_can_shoot(true){
//}

bool Tank::IsAbleToShoot() const {
    return m_can_shoot;
}


float Tank::GetCurrentHealth() const {
    return m_current_health;
}


int Tank::GetCurrentNumsOfLife () const{
    return m_num_of_lives;
}

int Tank::GetX() const{
    return m_x;
}

int Tank:: GetY() const{
    return m_y;
}

int Tank::GetSpeed() const{
    return m_speed;
}

int Tank::GetCurrentNumsOfRockets() const{
    return m_num_of_rockets;

}
int Tank::GetScore() const{
    return m_score;
}

void Tank::DecreaseHealth(int health) {
    m_current_health -= health;
}

void Tank::IncreaseHealth(int health) {
    m_current_health += health;
}

bool Tank::IsDead() const {
    return m_num_of_lives==0;
}
void Tank::SetX(int x){
    m_x=x;
}

void Tank::SetY(int y){
    m_y=y;
}

void Tank::SetSpeed(int speed){
    m_speed=speed;
}

void Tank::SetCanShot(){
    if (m_can_shoot==0){
        m_can_shoot= false;
    }
    else{
        m_can_shoot=true;
    }
}


void Tank::DecreaseNumOfLife(){
    if (m_current_health<=0){
        m_num_of_lives-=1;
    }
}


void Tank::IncreaseNumOfLife(){
    m_num_of_lives+=1;
}


void Tank::shoot() {
    m_num_of_rockets-=1;
}

//const std::vector<Rocket> &Tank::getRockets() const {
//    return m_rockets;
//}

//void Tank::setRockets(const std::vector<Rocket> &mRocket) {
//    m_rockets = mRocket;
//}
void Tank::IncreaseScore(int score){
    m_score+=score;
}
