#include "code/include/Rocket.hpp"
#include "code/include/World.hpp"
#include <QDebug>
#include <iostream>
#include <cmath>

int Rocket::rakete_tenka_0;
int Rocket::rakete_tenka_1;


 //Rocket::Rocket(float x, float y, float r, int rocket_power,Input* input,int id, int x_v, int y_v, qreal rot, Tank *t)
 //   :m_x(x),m_y(y),m_r(r),m_id(id),m_pravac_x(x_v),m_pravac_y(y_v),m_rotation(rot), t(t)


//u rocket power ce se prosledjivati enum u zavisnoti koju jacinu poseduje tenk
Rocket::Rocket(float x, float y, float r, const Rocket_type rocket_type,int id, int x_v, int y_v, qreal rot,Tank *t)
    :m_x(x),m_y(y),m_r(r),m_rocket_type(rocket_type),m_id(id),m_pravac_x(x_v),m_pravac_y(y_v),m_rotation(rot),m_t(t)
{
    setRotation(rot);
    setPos(m_x, m_y);

    if(id == 0) //prvi tenk pravi raketu
        rakete_tenka_0 += 1;
    else //drugi tenk pravi raketu
        rakete_tenka_1 += 1;


    if(m_rocket_type == Rocket_type::Low_power) {
        m_boja = Qt::red;
        m_rocket_power = 25;
    }
    else if(m_rocket_type == Rocket_type::Medium_power){
        m_boja = Qt::yellow;
        m_rocket_power = 50;
    }
    else if(m_rocket_type == Rocket_type::High_power){
        m_boja = Qt::blue;
        m_rocket_power = 100;
    }
    else throw "Nepodrzana jacina metka";


    //dodat nezavisan timer za pomeranje rakete-
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    timer->start(30); //move se poziva na svakih 30ms

}
Rocket::~Rocket()
{
    delete timer;
}
void Rocket::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

//    painter->setBrush(m_boja);
//    painter->drawEllipse(m_x,m_y,m_r,m_r);

    painter->setBrush(m_boja);
    painter->drawEllipse(0, 0, m_r, m_r);
}

QRectF Rocket::boundingRect() const
{
    return QRectF(-0.5, -0.5, m_r + 1, m_r + 1);
}

QPainterPath Rocket::shape() const
{
    QPainterPath OuterPath;
    OuterPath.setFillRule(Qt::WindingFill);
    OuterPath.addEllipse(-0.5, -0.5, m_r + 1, m_r + 1);

    return OuterPath;
}

void Rocket::move()
{
//    timer += 0.001;

//    QList<QGraphicsItem> colliding_items = collidingItems();
//    for (int i = 0; i < colliding_items.size(); i++)
//        if (typeid(*(colliding_items[i])) == typeid(Wall))

/*
    setPos(x() - m_pravac_x,y() - m_pravac_y);

    if (!scene()->collidingItems(this).isEmpty()) {
       //if (typeid(scene()->collidingItems(this).first()) == 0){
            setPos(x() + m_pravac_x,y() + m_pravac_y);
            auto new_x = m_pravac_x * cos(m_rotation) - m_pravac_y * sin(m_rotation);
            auto new_y = m_pravac_x * sin(m_rotation) + m_pravac_y * cos(m_rotation);
            m_pravac_x = new_x;
            m_pravac_y = new_y;
            //m_rotation = m_rotation;
            setPos(x() - m_pravac_x,y() - m_pravac_y);
        //}
    }
*/
    if(World::world_pause){
        return;
    }

    if (!scene()->collidingItems(this).isEmpty()) {

        int wall_collision_count = 0;
        std::vector<Wall *> wall_colided;
        QList<QGraphicsItem *> items = scene()->collidingItems(this);
//        bool bounced_once = false;
        for (auto item : items) {
//            if (item->type()==4){
//                delete this;
//                scene()->removeItem(this);
//                if(m_id == 0)
//                    rakete_tenka_0 -= 1;
//                else
//                    rakete_tenka_1 -= 1;
//            }
            //0 je id elementa Wall
            if (item->type() == 0) {
                wall_collision_count++;
                wall_colided.push_back(qgraphicsitem_cast<Wall*>(item));
//                first = false;
            }

            //1 je id elementa Tank
            if (item->type() == 1 && !first){
                if(m_id == 0)
                    rakete_tenka_0 -= 1;
                else
                    rakete_tenka_1 -= 1;
                Tank *t = qgraphicsitem_cast<Tank*>(item);
                t->decrease_health(this->m_rocket_power);
                if(t->get_current_health() <= 0)
                    t->destroy();

                //ovaj return je potreban jer ce doci do seg-faulta jer dole opet pristupamo raketi
                scene()->removeItem(this);
                delete this;
                return;
            }
        }

        if (wall_collision_count == 1) {
            Wall *w  = wall_colided.front();

            if (first) {
                center = mapToScene(m_r / 2, m_r / 2);
                center.rx() += m_pravac_x;
                center.ry() += m_pravac_y;

                m_pravac_x = -m_pravac_x / 2;
                m_pravac_y = -m_pravac_y / 2;
            }
            else {
                if(w->isVertical()) {

                    if ((center.y() + (m_r / 2)) < w->getY() || (center.y() - (m_r / 2)) > w->getY() + w->getHeight()) {
                        auto new_x = m_pravac_x;
                        auto new_y = -m_pravac_y;

                        m_pravac_x = new_x;
                        m_pravac_y = new_y;
                    }
                    else if(((center.y() - (m_r / 2)) < w->getY() && (center.y() + (m_r / 2)) > w->getY()) || ((center.y() - (m_r / 2)) < w->getY() + w->getHeight() && (center.y() + (m_r / 2)) > w->getY() + w->getHeight())) {
                        m_pravac_x = -m_pravac_x;
                        m_pravac_y = -m_pravac_y;
                    }
                    else {
                        auto new_x = -m_pravac_x;
                        auto new_y = m_pravac_y;

                        m_pravac_x = new_x;
                        m_pravac_y = new_y;
                    }
                }
                else {
                    if ((center.x() + (m_r / 2)) < w->getX() || (center.x() - (m_r / 2)) > w->getX() + w->getWidth()) {
                        auto new_x = -m_pravac_x;
                        auto new_y = m_pravac_y;

                        m_pravac_x = new_x;
                        m_pravac_y = new_y;
                    }

                    else {
                        auto new_x = m_pravac_x;
                        auto new_y = -m_pravac_y;

                        m_pravac_x = new_x;
                        m_pravac_y = new_y;

                    }
                }
            }
        }
        else if(wall_collision_count == 2) {
            Wall *w1  = wall_colided.at(0);
            Wall *w2  = wall_colided.at(1);

            if (first) {
                center = mapToScene(m_r / 2, m_r / 2);
                center.rx() += m_pravac_x;
                center.ry() += m_pravac_y;

                m_pravac_x = -m_pravac_x / 2;
                m_pravac_y = -m_pravac_y / 2;
            }
            else {
                if ((center.y() < w1->getY() && center.y() < w2->getY()) || (center.y() > (w1->getY() + w1->getHeight()) && center.y() > (w2->getY() + w2->getHeight()))) {
                    if(w1->isVertical()) {
                        if ((center.y() + (m_r / 2)) < w1->getY() || (center.y() - (m_r / 2)) > w1->getY() + w1->getHeight()) {
                            auto new_x = m_pravac_x;
                            auto new_y = -m_pravac_y;

                            m_pravac_x = new_x;
                            m_pravac_y = new_y;
                        }
                        else {
                            auto new_x = -m_pravac_x;
                            auto new_y = m_pravac_y;

                            m_pravac_x = new_x;
                            m_pravac_y = new_y;
                        }
                    }
                    else {
                        if ((center.x() + (m_r / 2)) < w1->getX() || (center.x() - (m_r / 2)) > w1->getX() + w1->getWidth()) {
                            auto new_x = -m_pravac_x;
                            auto new_y = m_pravac_y;

                            m_pravac_x = new_x;
                            m_pravac_y = new_y;
                        }

                        else {
                            auto new_x = m_pravac_x;
                            auto new_y = -m_pravac_y;

                            m_pravac_x = new_x;
                            m_pravac_y = new_y;

                        }
                    }
                }
                else {
                    m_pravac_x = -m_pravac_x;
                    m_pravac_y = -m_pravac_y;
                }
            }
        }
    }

    center = mapToScene(m_r / 2, m_r / 2);

    if(first) {
        first = false;
        m_x -= m_pravac_x / 8 * 5;
        m_y -= m_pravac_y / 8 * 5;
    }

    m_x -= m_pravac_x;
    m_y -= m_pravac_y;
    setPos(m_x, m_y);


    //unistavmo raketu posle izvesnog vremena otprilike oko 5 sekundi
    m_life_time++;
    if(m_life_time > 150)
    {
        if(m_id == 0)
            rakete_tenka_0 -= 1;
        else
            rakete_tenka_1 -= 1;

        scene()->removeItem(this);
        delete this;
        qDebug() << "Raketa je unistena: " << rakete_tenka_0;
    }
}


int Rocket::type() const{
    return 3;
}

