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
#include <code/include/SuperPower.hpp>
#include <QRandomGenerator>

#define ANGLE 9
#define TANK_W 26
#define TANK_H 30
#define PIPE_H 10
#define PIPE_W 6
#define ROCKET_RADIUS 4
#define MAX_ROCKET 5

float timer0 = 0.1;
float timer1 = 0.1;
int r_power = 0;
int timer2 = 0;

Tank::Tank(int id,QColor color, float x, float y, Input *input)
    :m_id(id),m_color(color), m_x(x), m_y(y), m_input(input)
{
    setTransformOriginPoint(TANK_W / 2, (TANK_H / 2) + PIPE_H / 2);
    setPos(m_x, m_y);
    m_health = 100;
    m_num_of_rockets = 0;

    //u tenku pravim muziku za raketu
    //inicijalizujemo sound za rocket
    rocket_sound = new QMediaPlayer();
    rocket_sound->setMedia(QUrl("qrc:/resources/sounds/rocket_sound.wav"));
    rocket_sound->setVolume(10);
}

Tank::~Tank()
{
    delete rocket_sound;
}

void Tank::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPixmap tank_pixmap;

    if (m_id == 0) {
        tank_pixmap = QPixmap(":/resources/images/red_tank.png");
    }
    else {
        tank_pixmap = QPixmap(":/resources/images/blue_tank.png");
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(tank_pixmap);
    painter->drawRect(0, PIPE_H / 2, TANK_W, TANK_H);

    painter->setBrush(QPixmap(":/resources/images/tank_pipe.png"));
    painter->drawRect((TANK_W - PIPE_W) / 2, 0, PIPE_W, PIPE_H);
}

QRectF Tank::boundingRect() const
{
//    return QRectF(0, 0, 30, 30);
    return QRectF(-0.5, -0.5, TANK_W + 1, TANK_H + (PIPE_H / 2) + 1);
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
QPainterPath Tank::shape() const {
    QPainterPath OuterPath;
    OuterPath.setFillRule(Qt::WindingFill);
    OuterPath.addRect((TANK_W - PIPE_W) / 2, 0, PIPE_W, PIPE_H / 2);
    OuterPath.addRect(0, PIPE_H / 2, TANK_W, TANK_H);

    return OuterPath;
}

void Tank::destroy() {
    this->destroyed = true;
    //scene()->removeItem(this);
    delete this;
   // return;
    //end_of_round();
}

void Tank::move_forward() {
    m_x -= x_v;
    m_y -= y_v;
    setPos(m_x, m_y);

    if(!scene()->collidingItems(this).isEmpty()) {
        QList<QGraphicsItem *> col_list = scene()->collidingItems(this);
        for(auto item : col_list) {
            if (item->type() == 4 ) {
               SuperPower *sp  = qgraphicsitem_cast<SuperPower*>(item);
               if(sp->getType() == QString("speed")){
                   this->SetSpeed(1);
                   timer0=0.1;
               }
               else if(sp->getType() == QString("superpower")){
                   m_power=1;
                   timer1=0.1;
                   r_power=1;
               }
               else if(sp->getType() == QString("health")){
                   this->increase_health(100);
               }
               scene()->removeItem(item);
               delete item;
            }
            else {
                while(this->collidesWithItem(item) && item->type() != 3) {
                    m_x += x_v / 5;
                    m_y += y_v / 5;
                    setPos(m_x, m_y);
                }
            }
            col_list = scene()->collidingItems(this);
        }
    }
}

void Tank::move_backward() {
    m_x += x_v / 5 * 2;
    m_y += y_v / 5 * 2;
    setPos(m_x, m_y);

    if(!scene()->collidingItems(this).isEmpty()) {
        QList<QGraphicsItem *> col_list = scene()->collidingItems(this);
        for(auto item : col_list) {
            if (item->type() == 4 ) {
               SuperPower *sp  = qgraphicsitem_cast<SuperPower*>(item);
               if(sp->getType() == QString("speed")){
                   this->SetSpeed(1);
                   timer0=0.1;
               }
               else if(sp->getType() == QString("superpower")){
                   m_power=1;
                   timer1=0.1;
                   r_power=1;
               }
               else if(sp->getType() == QString("health")){
                   this->increase_health(100);
               }
               scene()->removeItem(item);
               delete item;
            }
            else {
                while(this->collidesWithItem(item) && item->type() != 3) {
                    m_x -= x_v / 5;
                    m_y -= y_v / 5;
                    setPos(m_x, m_y);
                }
            }
            col_list = scene()->collidingItems(this);
        }
    }
}

void Tank::rotate(float angle) {
    setRotation(rotation() + angle);

    if (!scene()->collidingItems(this).isEmpty()) {
        QList<QGraphicsItem *> col_list = scene()->collidingItems(this);
        for(auto item : col_list) {

            if (item->type() == 4) {
               SuperPower *sp  = qgraphicsitem_cast<SuperPower*>(item);
               if(sp->getType() == QString("speed")){
                   this->SetSpeed(1);
                   timer0=0.1;
               }
               else if(sp->getType() == QString("superpower")){
                   m_power=1;
                   timer1=0.1;
                   r_power=1;
               }
               else if(sp->getType() == QString("health")){
                   this->increase_health(100);
               }

               scene()->removeItem(item);
               delete item;
            }

            else {
                setRotation(rotation() - angle);
            }
        }
    }
}
bool Tank::is_destroyed(){
    return this->destroyed;
}

void Tank::set_end_of_round(){
    m_end_of_round = true;
}

QString Tank::get_name() const{
    return m_name;
}
void Tank::set_name(const QString &name){
    m_name = name;
}

int Tank::get_current_health() const{
    return m_health;
}

bool Tank::is_pause(){
    return m_pause;
}

void Tank::advance()
{
    m_pause = m_input->pause;
//    qDebug() << "Pause: " << m_pause;

    if(m_pause)
        return;


    timer2 += 1;
    if(m_end_of_round==0){
    if(timer2 % 700 == 0){
        int rand = QRandomGenerator::global()->bounded(3);
        qDebug() << rand;
        qDebug() << "SuperPower";

        if(rand == 0){
            SuperPower *sp1 = new SuperPower(QString("superpower"),QRandomGenerator::global()->bounded(1240),QRandomGenerator::global()->bounded(600),30);
            scene()->addItem(sp1);
        }
        else if(rand == 1){
            SuperPower *sp2 = new SuperPower(QString("health"),QRandomGenerator::global()->bounded(1240),QRandomGenerator::global()->bounded(600),30);
            scene()->addItem(sp2);
        }
        else if(rand == 2){
            SuperPower *sp3 = new SuperPower(QString("speed"),QRandomGenerator::global()->bounded(1240),QRandomGenerator::global()->bounded(600),30);
            scene()->addItem(sp3);
        }
    }
    }
    if(timer0 > 10){
        m_speed = 0;
    }

    if(timer1 > 10){
        r_power = 0;
        m_power = 0;
    }

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
       //this->destroy(); 
        return;
        // ovde bi trebala neka animacija eksplozije da se napravi
    }

    QPointF pos_vector_x = mapToScene(0, 0);
    QPointF pos_vector_y = mapToScene(0, 1);

    x_v = pos_vector_y.rx() - pos_vector_x.rx();
    y_v = pos_vector_y.ry() - pos_vector_x.ry();

    float n = sqrt(pow(x_v, 2) + pow(y_v, 2));
    x_v /= n;
    y_v /= n;

    float r_speed_x = x_v;
    float r_speed_y = y_v;

    x_v *= 5.0;
    y_v *= 5.0;

//kombinacije tastera u narednoj if naredbi nisu semanticki ispravne, nema ih smisla kombinovati
//    if ((left && right) || (up && down && (left || right))) {
//        return;
//    }

    if(m_speed == 1){
        timer0+=0.1;
        x_v*=1.8;
        y_v*=1.8;
        r_speed_x*=1.8;
        r_speed_y*=1.8;
    }
    if(m_power == 1){
        timer1+=0.1;
        r_speed_x*=1.8;
        r_speed_y*=1.8;
    }

///////////////////////////////////////////////////////////////////
    if (left && up) {
        move_forward();
        rotate(-ANGLE);
    }
    else if (right && up) {
        move_forward();
        rotate(ANGLE);
    }

    else if (left && down) {
        move_backward();
        rotate(ANGLE);
    }

    else if (right && down) {
        move_backward();
        rotate(-ANGLE);
    }

    else if (up) {
        move_forward();
    }

    else if (left) {
        rotate(-ANGLE);
    }

    else if (right) {
        rotate(ANGLE);
    }

    else if (down) {
        move_backward();
    }

///////////////////////////////////////////////////////////////////
    //ako je pritisnut space or enter pravi se raketa
    if(launch){
        //trebaju nam koordinate tenk da bismo napravili raketu
//        float tank_x_position = this->x() - 10*x_v;
//        float tank_y_position = this->y() - 10*y_v;

        QPointF rckt_pos = mapToScene((TANK_W / 2) - ROCKET_RADIUS, -2 * ROCKET_RADIUS - 0.1);
        //cetvrti argument r_power
        Rocket *rocket = new Rocket(rckt_pos.x(), rckt_pos.y(), ROCKET_RADIUS, m_tank_rocket_type, m_id, 8 * r_speed_x , 8 * r_speed_y, rotation(), this);

        if(m_id == 0 && rocket->rakete_tenka_0 <= MAX_ROCKET){
               qDebug() << "Raketa 0 je napravljena";
               scene()->addItem(rocket);
               rocket->setParentItem(nullptr); //osiguravamo da rocket nema roditelja
//               update();
               rocket->move();
//               rocket->setPos(rocket->x(),rocket->y());

               //zvuk rakete se pusta ukoliko je moguce napraviti raketu
               if(rocket_sound->state() == QMediaPlayer::PlayingState)
                    rocket_sound->setPosition(0);
               else if(rocket_sound->state() == QMediaPlayer::StoppedState)
                   rocket_sound->play();
        }
        else if(m_id == 1 && rocket->rakete_tenka_1 <= MAX_ROCKET){
               qDebug() << "Raketa 1 je napravljena";
               scene()->addItem(rocket);
               rocket->setParentItem(nullptr); //osiguravamo da rocket nema roditelja
//               update();
               rocket->move();
//               rocket->setPos(rocket->x(),rocket->y());

               //zvuk rakete se pusta ukoliko je moguce napraviti raketu
               if(rocket_sound->state() == QMediaPlayer::PlayingState)
                    rocket_sound->setPosition(0);
               else if(rocket_sound->state() == QMediaPlayer::StoppedState)
                   rocket_sound->play();
        }
        else{
            if(m_id == 1)
                rocket->rakete_tenka_1 -= 1;
            if(m_id == 0)
                rocket->rakete_tenka_0 -= 1;
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
int Tank::get_score() const{
    return m_score;
}

void Tank::set_score(int score){
    m_score = score;
}

void Tank::decrease_health(int health) {
    m_health -= health;
}

void Tank::increase_health(int health) {
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
