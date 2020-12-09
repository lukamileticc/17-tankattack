#include "../include/Tank.hpp"
#include "../include/Rocket.hpp"
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <cmath>
#include <iostream>
#include <QDebug>
#define UNUSED(x) (void)(x)
#define ANGLE 10
#define TANK_W 26
#define TANK_H 30
#define MAX_ROCKET 7


Tank::Tank(int id,QColor color, float x, float y, Input *input)
    :m_id(id),m_color(color), m_x(x), m_y(y), m_input(input)
{
    setTransformOriginPoint(TANK_W / 2, TANK_H / 2);
    setPos(m_x, m_y);
    m_health = 100;


}

void Tank::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    UNUSED(option);
    UNUSED(widget);

    painter->setBrush(m_color);

    painter->drawRect(0, 0, TANK_W, TANK_H);
    //painter->drawRect(m_x, m_y, 30, 30);
}

QRectF Tank::boundingRect() const
{
//    return QRectF(0, 0, 30, 30);
    return QRectF(-0.5, -0.5, TANK_W + 1, 1 + TANK_H);
}
float Tank::getXposition() const
{
    return m_x;
}
float Tank::getYposition() const
{
    return m_y;
}

int Tank::type() const{
    return 1;
}

void Tank::destroy() {
    this->destroyed = true;
    scene()->removeItem(this);
    //delete this;
    //end_of_round();
}

bool Tank::is_destroyed(){
    return this->destroyed;
}

void Tank::advance()
{
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
        launch = m_input->k_space;
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
        launch = m_input->k_enter;
    }
    else{
        std::cout << "Greska, nepostojeci tenk" << std::endl;
    }

    if (this->is_destroyed()){
        return;
        // ovde bi trebala neka animacija eksplozije da se napravi
    }

    QPointF pos_vector_x = mapToScene(0, 0);
    QPointF pos_vector_y = mapToScene(0, TANK_H);

    x_v = pos_vector_y.rx() - pos_vector_x.rx();
    y_v = pos_vector_y.ry() - pos_vector_x.ry();

    float n = sqrt(pow(x_v, 2) + pow(y_v, 2));
    x_v /= n;
    y_v /= n;

    x_v *= 4.0;
    y_v *= 4.0;

//kombinacije tastera u narednoj if naredbi nisu semanticki ispravne, nema ih smisla kombinovati
//    if ((left && right) || (up && down && (left || right))) {
//        return;
//    }

    if (left && up) {
        setRotation(rotation() - ANGLE);
        m_x -= x_v;
        m_y -= y_v;
        setPos(m_x, m_y);

        if (!scene()->collidingItems(this).isEmpty()) {
            setRotation(rotation() + ANGLE);
            m_x += x_v;
            m_y += y_v;
            setPos(m_x, m_y);
//            return;
        }
    }
    else if (right && up) {
        setRotation(rotation() + ANGLE);
        m_x -= x_v;
        m_y -= y_v;
        setPos(m_x, m_y);

        if (!scene()->collidingItems(this).isEmpty()) {
            setRotation(rotation() - ANGLE);
            m_x += x_v;
            m_y += y_v;
            setPos(m_x, m_y);
//            return;
        }
    }

    else if (up) {
        m_x -= x_v;
        m_y -= y_v;
        setPos(m_x, m_y);

        if (!scene()->collidingItems(this).isEmpty()) {
            m_x += x_v;
            m_y += y_v;
            setPos(m_x, m_y);
        }
    }

//potrebno je jos dodati if-ove za (down && left) i (down && right)
    else if (left && down){
        setRotation(rotation() + ANGLE);
        m_x += x_v;
        m_y += y_v;
        setPos(m_x,m_y);

        if (!scene()->collidingItems(this).isEmpty()){
            setRotation(rotation() - ANGLE);
            m_x -= x_v;
            m_y -= y_v;
            setPos(m_x,m_y);
        }
    }

    else if (right && down){
        setRotation(rotation() - ANGLE);
        m_x += x_v;
        m_y += y_v;
        setPos(m_x,m_y);

        if (!scene()->collidingItems(this).isEmpty()){
            setRotation(rotation() + ANGLE);
            m_x -= x_v;
            m_y -= y_v;
            setPos(m_x,m_y);
        }
    }

    else if (left) {
        setRotation(rotation() - ANGLE);

        if (!scene()->collidingItems(this).isEmpty()) {
            setRotation(rotation() + ANGLE);
            std::cout << scene()->collidingItems(this).size() << std::endl;
//            return;
        }
    }

    else if (right) {
        setRotation(rotation() + ANGLE);

        if (!scene()->collidingItems(this).isEmpty()) {
            setRotation(rotation() - ANGLE);
            std::cout << scene()->collidingItems(this).size() << std::endl;
//            return;
        }
    }

    else if (down) {
        m_x += x_v;
        m_y += y_v;
        setPos(m_x, m_y);

        if (!scene()->collidingItems(this).isEmpty()) {
            m_x -= x_v;
            m_y -= y_v;
            setPos(m_x, m_y);
        }
    }

///////////////////////////////////////////////////////////////////
    //ako je pritisnut space or enter pravi se raketa
    if(launch){
        //trebaju nam koordinate tenk da bismo napravili raketu
//        float tank_x_position = this->x() - 10*x_v;
//        float tank_y_position = this->y() - 10*y_v;

        QPointF rckt = mapToScene(ceil((TANK_W / 2) - 5), -6);
        float rckt_pos_x = rckt.rx() - 5;
        float rckt_pos_y = rckt.ry() - 5;


        Rocket *rocket = new Rocket(rckt_pos_x, rckt_pos_y, 10, 0, m_id, 2 * x_v , 2 * y_v, rotation());

        if(m_id == 0 && rocket->rakete_tenka_0 <= MAX_ROCKET){
               qDebug() << "Raketa 0 je napravljena";
               scene()->addItem(rocket);
               rocket->setParentItem(nullptr); //osiguravamo da rocket nema roditelja
//               rocket->setPos(rocket->x(),rocket->y());
        }
        else if(m_id == 1 && rocket->rakete_tenka_1 <= MAX_ROCKET){
               qDebug() << "Raketa 1 je napravljena";
               scene()->addItem(rocket);
               rocket->setParentItem(nullptr); //osiguravamo da rocket nema roditelja
//               rocket->setPos(rocket->x(),rocket->y());
        }
        else{
            if(m_id == 1) rocket->rakete_tenka_1 -= 1;
            if(m_id == 0) rocket->rakete_tenka_0 -= 1;
            delete rocket;
        }
        if (m_id == 0)
            m_input->k_space = false;
        if (m_id == 1)
            m_input->k_enter = false;
    }

}

//void Tank::keyPressEvent(QKeyEvent *event) {

////komentar je potrebno ukloniti ukoliko zelimo da eliminisemo auto-repeat tastera
////    if (event->isAutoRepeat()) {
////            return;
////    }
///
///
///
///

//    if (event->key() == Qt::Key_W) up = true;
//    if (event->key() == Qt::Key_S) down = true;
//    if (event->key() == Qt::Key_A) left = true;
//    if (event->key() == Qt::Key_D) right = true;
//    advance(0);
//}

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
    return m_health;
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
    m_health -= health;
}

void Tank::IncreaseHealth(int health) {
    m_health += health;
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
    if (m_health<=0){
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
