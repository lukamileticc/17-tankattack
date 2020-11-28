#include "../include/Tank.hpp"
#include "../include/rocket_1.hpp"

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QKeyEvent>


Tank::Tank(QColor color, float x, float y)
    :m_color(color), m_x(x), m_y(y)
{
}

void Tank::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(m_color);

    painter->drawRect(m_x, m_y, 30, 30);
}

QRectF Tank::boundingRect() const
{
    return QRectF(m_x, m_y, 40, 40);
}


bool Tank::IsAbleToShoot() const {
    return m_can_shoot;
}


float Tank::GetCurrentHealth() const {
    return m_current_health;
}


int Tank::GetCurrentNumsOfLife () const{
    return m_num_of_lives;
}

float Tank::GetX() const{
    return m_x;
}

float Tank:: GetY() const{
    return m_y;
}

float Tank::GetSpeed() const{
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
void Tank::SetX(float x){
    m_x=x;
}

void Tank::SetY(float y){
    m_y=y;
}

void Tank::SetSpeed(float speed){
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

//#############################################
void Tank::keyPressEvent(QKeyEvent *event){
//    qDebug() << "Tenk reaguje da je kliknuto na njega";

    if(event->key() == Qt::Key_Space){
        //pravimo metak
        Rocket *rocket = new Rocket(300,300,100,2);
        qDebug() << "Metak je napravljen";
//        m_view->scene()->addItem(rocket);
    }

}
