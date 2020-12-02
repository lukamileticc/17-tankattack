#include "../include/Tank.hpp"
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>
#include <QKeyEvent>
#include <cmath>
#include <iostream>
#define ANGLE 15


Tank::Tank(int id,QColor color, float x, float y, Input *input)
    :m_id(id),m_color(color), m_x(x), m_y(y), m_input(input)
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
    return QRectF(0, 0, 30, 30);

    //neophodan je precizniji boudnRect
    //return QRectF(0, 0, 1280, 70);
}

//void Tank::keyPressEvent(QKeyEvent *event) {

////komentar je potrebno ukloniti ukoliko zelimo da eliminisemo auto-repeat tastera
////    if (event->isAutoRepeat()) {
////            return;
////    }

//    if (event->key() == Qt::Key_W) up = true;
//    if (event->key() == Qt::Key_S) down = true;
//    if (event->key() == Qt::Key_A) left = true;
//    if (event->key() == Qt::Key_D) right = true;
//    advance(0);
//}

void Tank::advance(){
    if(m_id == 0){/*
        unsigned int commands = m_input->key_tank1;
        if((commands & key_up) == key_up){
            up = !up;
            std::cout << "W" << std::endl;
        }
        if((commands & key_down) == key_down)
            down = !down;
        if((commands & key_right) == key_right)
            right = !right;
        if((commands & key_left) == key_left)
            left = !left;*/
        up = m_input->k_w;
        down = m_input->k_s;
        right = m_input->k_d;
        left = m_input->k_a;
    }
    else if(m_id == 1){/*
        unsigned int commands = m_input->key_tank2;
        if((commands & key_up) == key_up)
            up = !up;
        if((commands & key_down) == key_down)
            down = !down;
        if((commands & key_right) == key_right)
            right = !right;
        if((commands & key_left) == key_left)
            left = !left;*/
        up = m_input->k_up;
        down = m_input->k_down;
        right = m_input->k_right;
        left = m_input->k_left;
    }
    else{
        std::cout << "Greska, nepostojeci tenk" << std::endl;
    }

    QPointF pos_vector_x = mapToScene(0, 0);
    QPointF pos_vector_y = mapToScene(0, 30);

    x_v = pos_vector_y.rx() - pos_vector_x.rx();
    y_v = pos_vector_y.ry() - pos_vector_x.ry();

    float n = sqrt(pow(x_v, 2) + pow(y_v, 2));
    x_v /= n;
    y_v /= n;

    x_v *= 2.0;
    y_v *= 2.0;


    if (right && up) {
//        rotation_angle += 10;
        setRotation(rotation() + ANGLE);

        m_x -= x_v;
        m_y -= y_v;
    }
    else if (left && up) {
//        rotation_angle -= 10;
        setRotation(rotation() - ANGLE);

        m_x -= x_v;
        m_y -= y_v;
    }
    else if(left) {
//        rotation_angle -= 10;
        setRotation(rotation() - ANGLE);
    }
    else if(right) {
//        rotation_angle += 10;
        setRotation(rotation() + ANGLE);
    }
    else if(up) {
        m_x -= x_v;
        m_y -= y_v;
    }
//    if(down) {
//        m_x += x_v;
//        m_y += y_v;
//    }

    setPos(m_x, m_y);
}

//void Tank::keyReleaseEvent(QKeyEvent *event) {
//    if (event->key() == Qt::Key_W) up = false;
//    if (event->key() == Qt::Key_S) down = false;
//    if (event->key() == Qt::Key_A) left = false;
//    if (event->key() == Qt::Key_D) right = false;
//    advance(0);
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
