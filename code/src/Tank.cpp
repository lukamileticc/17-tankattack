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
float timer1 = 0.1;
int r_power = 0;
int timer2 = 0;

Tank::Tank(int id,QColor color, float x, float y, Input *input, QColor host, QColor client)
    :m_id(id),m_color(color), m_x(x), m_y(y), m_input(input), m_HostColor(host), m_ClientColor(client)
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
    tank_hit = new QMediaPlayer();
    tank_hit->setMedia(QUrl("qrc:/resources/sounds/explosion.wav"));
    tank_hit->setVolume(15);
    m_health_bar_tank=new HealthBar(50,10);
    m_health_bar_tank->barFrame->setPos(m_x-10,m_y-30);
    m_health_bar_tank->bar->setPos(m_x-10,m_y-30);

    if(m_id==0){
            m_healt_bar=new HealthBar(40,704,350,50);
    }
    else{
            m_healt_bar=new HealthBar(770,704,350,50);
    }
    if(isMultiPlayer)
    {
        if(m_color != m_ClientColor)
        {
            m_Client = new Client();
            m_Client->connectToServer(QHostAddress::LocalHost, 1967);


        }

    }

}

Tank::~Tank()
{
    delete tank_hit;
    delete rocket_sound;
   //m_input se unistava na mestu gde su unistena oba tenk---> Na kraju svake runde!
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
    scene()->addItem(m_healt_bar->barFrame);
    scene()->addItem(m_healt_bar->bar);
    scene()->addItem(m_health_bar_tank->barFrame);
    scene()->addItem(m_health_bar_tank->bar);
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
    //ne smemo brisati tenk ovde jer posle moramo proveriti ovaj flag
    // da li je tenk unisten!
    delete m_health_bar_tank->bar;
    delete m_health_bar_tank->barFrame;
    scene()->removeItem(this);
//    delete this;
   // return;
    //end_of_round();
}

void Tank::move_forward() {
    m_x -= x_v;
    m_y -= y_v;
    setPos(m_x, m_y);
    m_health_bar_tank->barFrame->setPos(m_x-10,m_y-30);
    m_health_bar_tank->bar->setPos(m_x-10,m_y-30);

    if(!scene()->collidingItems(this).isEmpty()) {
        QList<QGraphicsItem *> col_list = scene()->collidingItems(this);
        for(auto item : col_list) {
            if (item->type() == 4 ) {
               SuperPower *sp  = qgraphicsitem_cast<SuperPower*>(item);
               if(sp->getType() == QString("superpower")){
                   m_power=1;
                   timer1=0.1;
                   r_power=1;
                   m_tank_rocket_type=Rocket_type::Medium_power;
               }
               else if(sp->getType() == QString("health")){
                   this->SetHealth(100);
               }
               scene()->removeItem(item);
               delete item;
            }
            else {
                while(this->collidesWithItem(item) && item->type() != 3) {
                    m_x += x_v / 5;
                    m_y += y_v / 5;
                    setPos(m_x, m_y);
                    m_health_bar_tank->barFrame->setPos(m_x-10,m_y-30);
                    m_health_bar_tank->bar->setPos(m_x-10,m_y-30);
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
    m_health_bar_tank->barFrame->setPos(m_x-10,m_y-30);
    m_health_bar_tank->bar->setPos(m_x-10,m_y-30);

    if(!scene()->collidingItems(this).isEmpty()) {
        QList<QGraphicsItem *> col_list = scene()->collidingItems(this);
        for(auto item : col_list) {
            if (item->type() == 4 ) {
               SuperPower *sp  = qgraphicsitem_cast<SuperPower*>(item);
               if(sp->getType() == QString("superpower")){
                   m_power=1;
                   timer1=0.1;
                   r_power=1;
                   m_tank_rocket_type=Rocket_type::Medium_power;
               }
               else if(sp->getType() == QString("health")){
                   this->SetHealth(100);
               }
               scene()->removeItem(item);
               delete item;
            }
            else {
                while(this->collidesWithItem(item) && item->type() != 3) {
                    m_x -= x_v / 5;
                    m_y -= y_v / 5;
                    setPos(m_x, m_y);
                    m_health_bar_tank->barFrame->setPos(m_x-10,m_y-30);
                    m_health_bar_tank->bar->setPos(m_x-10,m_y-30);
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
               if(sp->getType() == QString("superpower")){
                   m_power=1;
                   timer1=0.1;
                   r_power=1;
                   m_tank_rocket_type=Rocket_type::Medium_power;
               }
               else if(sp->getType() == QString("health")){
                   this->SetHealth(100);
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

    if(isMultiPlayer && m_color == m_ClientColor)
    {
        setPos(Client::getX_Primljeno(), Client::getY_Primljeno());
        rotate(Client::getAngle_Primljeno());
//        Client::nullifyX();
//        Client::nullifyY();
        Client::nullifyAngle();
        qDebug() << Client::getAngle_Primljeno() << "ANGLE PRIMLJENO";
       //return;
    }


//    timer2 += 1;
//    if(m_end_of_round==0){
//    if(timer2 % 500 == 0){
//        int rand = QRandomGenerator::global()->bounded(2);
//        qDebug() << rand;
//        qDebug() << "SuperPower";

//        if(rand == 0){
//            SuperPower *sp1 = new SuperPower(QString("superpower"),QRandomGenerator::global()->bounded(1240),QRandomGenerator::global()->bounded(600),30);
//            scene()->addItem(sp1);
//        }
//        else if(rand == 1){
//            SuperPower *sp2 = new SuperPower(QString("health"),QRandomGenerator::global()->bounded(1240),QRandomGenerator::global()->bounded(600),30);
//            scene()->addItem(sp2);
//        }
//        else if(rand == 2){
//            SuperPower *sp3 = new SuperPower(QString("speed"),QRandomGenerator::global()->bounded(1240),QRandomGenerator::global()->bounded(600),30);
//            scene()->addItem(sp3);
//        }
//    }
//    }

//    if(timer1 > 10){
//        r_power = 0;
//        m_power = 0;
//        m_tank_rocket_type=Rocket_type::Low_power;
//    }

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

    if(m_power == 1){
        timer1+=0.1;
        m_tank_rocket_type=Rocket_type::Medium_power;
    }
//    if(isMultiPlayer && m_color == m_ClientColor){
//    switch (Client::getMovement()) {
//    case 1:
//        up = true;
//        qDebug() << "gore";
//        Client::nullifyMovement();
//        break;
//    case 2:
//        down = true;
//        qDebug() << "dole";
//        Client::nullifyMovement();
//        break;
//    case 3:
//        left = true;
//        Client::nullifyMovement();
//        break;
//    case 4:
//        right = true;
//        qDebug() << "desno";
//        Client::nullifyMovement();
//        break;
//    case 5:
//        left = true;
//        up = true;
//        Client::nullifyMovement();
//        break;
//    case 6:
//        right = true;
//        up = true;
//        Client::nullifyMovement();
//        break;
//    case 7:
//        left = true;
//        down = true;
//        Client::nullifyMovement();
//        break;
//    case 8:
//        right = true;
//        down = true;
//        Client::nullifyMovement();
//        break;
//    default:
//        break;
//    }
//    }

///////////////////////////////////////////////////////////////////
    if (left && up) {
        move_forward();
        rotate(-ANGLE);

        if(isMultiPlayer && m_color != m_ClientColor)
        {
            m_Client->setPozicija_TenkaX(m_x);
            m_Client->setPozicija_TenkaY(m_y);
            m_Client->setAngle_Tenka(-ANGLE);
            m_Client->sendMessage("Proba");
        }
    }
    else if (right && up) {
        move_forward();
        rotate(ANGLE);

        if(isMultiPlayer && m_color != m_ClientColor)
        {
                m_Client->setPozicija_TenkaX(m_x);
                m_Client->setPozicija_TenkaY(m_y);
                m_Client->setAngle_Tenka(ANGLE);
                m_Client->sendMessage("Proba");

        }
    }

    else if (left && down) {
        move_backward();
        rotate(ANGLE);

        if(isMultiPlayer && m_color != m_ClientColor)
        {
            m_Client->setPozicija_TenkaX(m_x);
            m_Client->setPozicija_TenkaY(m_y);
            m_Client->setAngle_Tenka(ANGLE);
            m_Client->sendMessage("Proba");
        }
    }

    else if (right && down) {
        move_backward();
        rotate(-ANGLE);

        if(isMultiPlayer && m_color != m_ClientColor)
        {
            m_Client->setPozicija_TenkaX(m_x);
            m_Client->setPozicija_TenkaY(m_y);
            m_Client->setAngle_Tenka(-ANGLE);
            m_Client->sendMessage("Proba");
        }
    }

    else if (up) {
        move_forward();
        if(isMultiPlayer && m_color != m_ClientColor)
        {
            m_Client->setPozicija_TenkaX(m_x);
            m_Client->setPozicija_TenkaY(m_y);
            m_Client->setAngle_Tenka(0);
            m_Client->sendMessage("Proba");
        }
        qDebug() << "ovde";
    }

    else if (left) {
        rotate(-ANGLE);

        if(isMultiPlayer && m_color != m_ClientColor)
        {
            m_Client->setPozicija_TenkaX(m_x);
            m_Client->setPozicija_TenkaY(m_y);
            m_Client->setAngle_Tenka(-ANGLE);
            m_Client->sendMessage("Proba");
        }
    }

    else if (right) {
        rotate(ANGLE);

        if(isMultiPlayer && m_color != m_ClientColor)
        {
            m_Client->setPozicija_TenkaX(m_x);
            m_Client->setPozicija_TenkaY(m_y);
            m_Client->setAngle_Tenka(ANGLE);
            m_Client->sendMessage("Proba");
        }
    }

    else if (down) {
        move_backward();

        if(isMultiPlayer && m_color != m_ClientColor)
        {
            m_Client->setPozicija_TenkaX(m_x);
            m_Client->setPozicija_TenkaY(m_y);
            m_Client->setAngle_Tenka(0);
            m_Client->sendMessage("Proba");
        }
    }

    if(isMultiPlayer && m_color == m_ClientColor && Client::isOrderedToShoot()){
        launch = true;
        qDebug() << "Pucaj!";
        Client::setCantShoot();
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
        if(isMultiPlayer && m_color == m_HostColor)
            m_Client->sendMessage("Space");
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
    delete m_health_bar_tank->bar;
    m_health_bar_tank->bar= new QGraphicsRectItem(0,0,get_current_health()*0.5,10);
    m_health_bar_tank->bar->setPos(GetX()-10,GetY()-30);
    m_health_bar_tank->bar->setBrush(Qt::green);
    if(getId()==0){
        delete m_healt_bar->bar;
        m_healt_bar->bar= new QGraphicsRectItem(40,704,get_current_health()*3.5,50);
        m_healt_bar->bar->setBrush(Qt::green);
        QString str = "bottom-right-radius: 10px; top-right-radius: 0px";
    }
    else{
    delete m_healt_bar->bar;
    m_healt_bar->bar= new QGraphicsRectItem(770,704,get_current_health()*3.5,50);
    m_healt_bar->bar->setBrush(Qt::green);
    }
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

void Tank::SetHealth(float health)
{
    m_health=health;
    delete m_health_bar_tank->bar;
    m_health_bar_tank->bar= new QGraphicsRectItem(0,0,get_current_health()*0.5,10);
    m_health_bar_tank->bar->setPos(GetX()-10,GetY()-30);
    m_health_bar_tank->bar->setBrush(Qt::green);
    if(getId()==0){
        delete m_healt_bar->bar;
        m_healt_bar->bar= new QGraphicsRectItem(40,704,get_current_health()*3.5,50);
        m_healt_bar->bar->setBrush(Qt::green);
        QString str = "bottom-right-radius: 10px; top-right-radius: 0px";
    }
    else{
    delete m_healt_bar->bar;
    m_healt_bar->bar= new QGraphicsRectItem(770,704,get_current_health()*3.5,50);
    m_healt_bar->bar->setBrush(Qt::green);
    }
}
int Tank::getId()
{
    return m_id;
}
QMediaPlayer* Tank::get_explosion_sound() const
{
    return tank_hit;
}

void Tank::setColors()
{
    m_ClientColor = Qt::blue;
    m_HostColor = Qt::red;
}
