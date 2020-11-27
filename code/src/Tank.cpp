#include "../include/Tank.hpp"
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>
#include <QKeyEvent>
#include <cmath>
#define ANGLE 5


Tank::Tank(QColor color, float x, float y)
    :m_color(color), m_x(x), m_y(y)
{
    setTransformOriginPoint(15, 15);
    setPos(m_x, m_y);
}

void Tank::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(m_color);

    //painter->drawRect(m_x, m_y, 30, 30);
    painter->drawRect(0, 0, 30, 30);
}

QRectF Tank::boundingRect() const
{
    //return QRectF(m_x, m_y, 40, 40);

    //neophodan je precizniji boudnRect
    return QRectF(0, 0, 1280, 70);
}

void Tank::keyPressEvent(QKeyEvent *event) {

//komentar je potrebno ukloniti ukoliko zelimo da eliminisemo auto-repeat tastera
//    if (event->isAutoRepeat()) {
//            return;
//    }

    if (event->key() == Qt::Key_W) up = true;
    if (event->key() == Qt::Key_S) down = true;
    if (event->key() == Qt::Key_A) left = true;
    if (event->key() == Qt::Key_D) right = true;
    advance(0);
}

void Tank::advance(int step){

    QPointF p2 = mapToScene(0, 0);
    QPointF p3 = mapToScene(0, 15);

    x_v = p3.rx() - p2.rx();
    y_v = p3.ry() - p2.ry();

    float n = sqrt(pow(x_v, 2) + pow(y_v, 2));
    x_v /= n;
    y_v /= n;


    if (right && up) {
//        rotation_angle += 10;
        setRotation(rotation() + ANGLE);

        m_x -= x_v;
        m_y -= y_v;
    }
    if (left && up) {
//        rotation_angle -= 10;
        setRotation(rotation() - ANGLE);

        m_x -= x_v;
        m_y -= y_v;
    }
    if(left) {
//        rotation_angle -= 10;
        setRotation(rotation() - ANGLE);
    }
    if(right) {
//        rotation_angle += 10;
        setRotation(rotation() + ANGLE);
    }
    if(up) {
        m_x -= x_v;
        m_y -= y_v;
    }
//    if(down) {
//        m_x += x_v;
//        m_y += y_v;
//    }

    setPos(m_x, m_y);
}

void Tank::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_W) up = false;
    if (event->key() == Qt::Key_S) down = false;
    if (event->key() == Qt::Key_A) left = false;
    if (event->key() == Qt::Key_D) right = false;
    advance(0);
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
