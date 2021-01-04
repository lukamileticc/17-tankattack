#include "code/include/Rocket.hpp"
#include "code/include/World.hpp"
#include <QDebug>
#include <iostream>
#include <cmath>
#define ROCKET_SPEED 8

int Rocket::rakete_tenka_0;
int Rocket::rakete_tenka_1;

Rocket::Rocket(float x, float y, float r, const Rocket_type rocket_type,int id, int x_v, int y_v, qreal rot,Tank *t)
    :m_x(x),m_y(y),m_r(r),m_rocket_type(rocket_type),m_id(id),m_direction_x(x_v),m_direction_y(y_v),m_rotation(rot),m_t(t)
{
    setRotation(rot);
    setPos(m_x, m_y);

    if(id == 0) //prvi tenk pravi raketu
        rakete_tenka_0 += 1;
    else //drugi tenk pravi raketu
        rakete_tenka_1 += 1;


    if(m_rocket_type == Rocket_type::Low_power) {
        m_color = QColor(242, 208, 63);
        m_rocket_power = 25;
    }
    else if(m_rocket_type == Rocket_type::Medium_power){
        m_color = Qt::red;
        m_rocket_power = 50;
    }
    else throw "Nepodrzana jacina metka";


    //dodat nezavisan timer za pomeranje rakete-
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(33); //move se poziva na svakih 33ms
}

Rocket::~Rocket() {
    delete timer;
}

void Rocket::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(m_color);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(0, 0, 2 * m_r, 2 * m_r);
}

QRectF Rocket::boundingRect() const {
    return QRectF(0, 0, 2 * m_r,  2 * m_r);
}

QPainterPath Rocket::shape() const {
    QPainterPath OuterPath;
    OuterPath.setFillRule(Qt::WindingFill);
    OuterPath.addEllipse(0, 0, 2 * m_r, 2 * m_r);

    return OuterPath;
}

void Rocket::move() {
    if(World::world_pause) {
        return;
    }

    if (!scene()->collidingItems(this).isEmpty()) {

        int wall_collision_count = 0;
        std::vector<Wall *> walls_colided_with;
        QList<QGraphicsItem *> items = scene()->collidingItems(this);
        for (auto item : items) {

            //0 je id elementa Wall
            if (item->type() == 0) {
                wall_collision_count++;
                walls_colided_with.push_back(qgraphicsitem_cast<Wall*>(item));
            }

            //1 je id elementa Tank
            if (item->type() == 1){
                if(m_id == 0)
                    rakete_tenka_0 -= 1;
                else
                    rakete_tenka_1 -= 1;

                Tank *t = qgraphicsitem_cast<Tank*>(item);
                t->decrease_health(this->m_rocket_power);
                //ovaj zvuk moze i kad se tenk unisti a moze i svaki put kad ga pogodi raketa
                //svejejdno dogovoricemo se
                t->get_explosion_sound()->play();
                if(t->get_current_health() <= 0)
                    t->destroy();

                //ovaj return je potreban jer ce doci do seg-faulta jer dole opet pristupamo raketi
                scene()->removeItem(this);
                delete this;
                return;
            }
        }

        if (wall_collision_count == 1) {
            Wall *w  = walls_colided_with.front();

            if (first) {
                m_direction_x = -m_direction_x / 2;
                m_direction_y = -m_direction_y / 2;
            }
            else {
                if(w->isVertical()) {
                    if ((center.y() + m_r) < w->getY() || (center.y() - m_r) > w->getY() + w->getHeight()) {
                        m_direction_y = -m_direction_y;
                    }
                    else if ((center.x() + m_r) < w->getX() || (center.x() - m_r) > w->getX() + w->getWidth()) {
                        m_direction_x = -m_direction_x;
                    }
                    else {
                        m_direction_x = -m_direction_x;
                        m_direction_y = -m_direction_y;
                    }
                }
                else {
                    if ((center.x() + m_r) < w->getX() || (center.x() - m_r) > w->getX() + w->getWidth()) {
                        m_direction_x = -m_direction_x;
                    }
                    else if ((center.y() + m_r) < w->getY() || (center.y() - m_r) > w->getY() + w->getHeight()) {
                        m_direction_y = -m_direction_y;
                    }
                    else {
                        m_direction_x = -m_direction_x;
                        m_direction_y = -m_direction_y;
                    }
                }
            }
        }
        else if(wall_collision_count == 2) {
            Wall *w1  = walls_colided_with.at(0);
            Wall *w2  = walls_colided_with.at(1);

            if (first) {
                m_direction_x = -m_direction_x / 2;
                m_direction_y = -m_direction_y / 2;
            }
            else {
                bool first_expression = center.y() < w1->getY() && center.y() < w2->getY();
                bool second_expression = center.y() > (w1->getY() + w1->getHeight()) && center.y() > (w2->getY() + w2->getHeight());
                bool third_expression = center.x() < w1->getX() && center.x() < w2->getX();
                bool fourth_expression = center.x() > (w1->getX() + w1->getWidth()) && center.x() > (w2->getX() + w2->getWidth());

                if (first_expression || second_expression || third_expression || fourth_expression) {
                    if(w1->isVertical()) {
                        if ((center.y() + m_r) < w1->getY() || (center.y() - m_r) > w1->getY() + w1->getHeight()) {
                            m_direction_y = -m_direction_y;
                        }
                        else {
                            m_direction_x = -m_direction_x;
                        }
                    }
                    else {
                        if ((center.x() + m_r) < w1->getX() || (center.x() - m_r) > w1->getX() + w1->getWidth()) {
                            m_direction_x = -m_direction_x;
                        }
                        else {
                            m_direction_y = -m_direction_y;
                        }
                    }
                }
                else {
                    m_direction_x = -m_direction_x;
                    m_direction_y = -m_direction_y;
                }
            }
        }
    }

    center = mapToScene(m_r, m_r);

    if(first) {
        first = false;
        m_x -= (m_direction_x / ROCKET_SPEED) * 5;
        m_y -= (m_direction_y / ROCKET_SPEED) * 5;
    }

    m_x -= m_direction_x;
    m_y -= m_direction_y;
    setPos(m_x, m_y);


    //unistavmo raketu posle izvesnog vremena otprilike oko 5 sekundi
    m_life_time++;
    if(m_life_time > 300) {
        if(m_id == 0)
            rakete_tenka_0 -= 1;
        else
            rakete_tenka_1 -= 1;

        scene()->removeItem(this);
        delete this;
    }
}

int Rocket::type() const {
    return 3;
}
