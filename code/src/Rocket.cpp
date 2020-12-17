#include "code/include/Rocket.hpp"
#include <QDebug>
#include <cmath>

int Rocket::rakete_tenka_0 = 0;
int Rocket::rakete_tenka_1 = 0;

float timer = 0.001;
//u rocket power ce se prosledjivati enum u zavisnoti koju jacinu poseduje tenk
Rocket::Rocket(float x, float y, float r, int rocket_power,Input* input,int id, int x_v, int y_v, qreal rot, Tank *t)
    :m_x(x),m_y(y),m_r(r),m_id(id),m_pravac_x(x_v),m_pravac_y(y_v),m_rotation(rot), t(t)
{
    setRotation(rot);
    setPos(m_x, m_y);

    if(id == 0) //prvi tenk pravi raketu
        rakete_tenka_0 += 1;
    else //drugi tenk pravi raketu
        rakete_tenka_1 += 1;


    if(rocket_power == 0) m_boja = Qt::yellow;
    else if(rocket_power == 1) m_boja = Qt::red;
    else if(rocket_power == 2) m_boja = Qt::blue;
    else throw "Nepodrzana jacina metka";


    timer = 0.001;
    connect(input->timer,SIGNAL(timeout()),this,SLOT(move()));
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
    timer += 0.001;

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
    QPointF center_old = mapToScene(m_r / 2, m_r / 2);
    m_x -= m_pravac_x;
    m_y -= m_pravac_y;
    setPos(m_x, m_y);

    if (!scene()->collidingItems(this).isEmpty()) {

        bool bounced_once = false;
        QList<QGraphicsItem *> items = scene()->collidingItems(this);

        for (auto item : items) {
            //0 je id elementa Wall
            if(item->type()==4){
                scene()->removeItem(this);
                delete this;
                if(m_id == 0)
                    rakete_tenka_0 -= 1;
                else
                    rakete_tenka_1 -= 1;
            }
            if (item->type() == 0 && !bounced_once) {

                if(timer < 0.0021){
//                    if(m_id == 0)
//                        rakete_tenka_0 -= 1;
//                    else
//                        rakete_tenka_1 -= 1;

//                scene()->removeItem(this);
//                delete this;
//                qDebug() << "Raketa je unistena: " << rakete_tenka_0;


                }
                bounced_once = true;
                Wall *w  = qgraphicsitem_cast<Wall*>(item);

                float cent_x = center_old.x();
                float cent_y = center_old.y();

                if(w->isVertical()) {
                    if ((cent_y + (m_r / 2)) < w->getY() || (cent_y - (m_r / 2)) > w->getY() + w->getHeight()) {
                            auto new_x = m_pravac_x;
                            auto new_y = -m_pravac_y;

                            m_pravac_x = new_x;
                            m_pravac_y = new_y;

                            if(timer < 0.0021){
                                //pozicija tenka da znamo da li raketu da gurnemo
                                //napred ili nazad, gore ili dole
                                if(t->getXposition() > w->getCoordinates().first){
                                m_x += 7;
                                //m_y -= 7;
                                }
                                else {
                                    m_x -= 7;
                                    //m_y -= 7;
                                }
                            setPos(m_x, m_y);
                            qDebug() << "DaVert dole";

                            }

                    }
                    else {
                            auto new_x = -m_pravac_x;
                            auto new_y = m_pravac_y;

                            m_pravac_x = new_x;
                            m_pravac_y = new_y;

                            if(timer < 0.0021){
                                if(t->getXposition() > w->getCoordinates().first){
                                m_x += 7;
                                //m_y -= 5;
                                }
                                else {
                                    m_x -= 7;
                                    //am_y -= 5;
                                }
                            setPos(m_x, m_y);
                            qDebug() << "DaVert gore";

                            }
                    }
                }
                else {
                    if ((cent_x + (m_r / 2)) < w->getX() || (cent_x - (m_r / 2)) > w->getX() + w->getWidth()) {

                            auto new_x = -m_pravac_x;
                            auto new_y = m_pravac_y;

                            m_pravac_x = new_x;
                            m_pravac_y = new_y;

                            if(timer < 0.0021){
                                if(t->getYposition() > w->getCoordinates().second){
                                m_y += 7;

                                }
                                else {
                                    m_y -= 7;

                                }
                            setPos(m_x, m_y);
                            qDebug() << "DaHoriz";

                            }

                    }

                    else {


                        auto new_x = m_pravac_x;
                        auto new_y = -m_pravac_y;

                        m_pravac_x = new_x;
                        m_pravac_y = new_y;


                        if(timer < 0.0021){

                            if(t->getYposition() > w->getCoordinates().second){
                            m_y += 7;
                            }
                            else {
                                m_y -= 7;
                            }
                        setPos(m_x, m_y);
                        qDebug() << "DaHoriz";

                        }
                    }
                }
            }
            //1 je id elementa Tank
            if (item->type() == 1){
                Tank *t = qgraphicsitem_cast<Tank*>(item);
                t->is_destroyed = true;
                delete this;
                return;
            }
        }
    }
//        else if(scene()->collidingItems(this).size() == 2){
//            // za slucaj da udari u dva zida na spoju -> odbija se od onog u kojeg je prvo udarila
//            if (scene()->collidingItems(this)[0]->type() == 0 &&
//                scene()->collidingItems(this)[1]->type() == 0){
//                Wall *w  = qgraphicsitem_cast<Wall*>(scene()->collidingItems(this).first());

//                if(w->isVertical()) {
//                    auto new_x = m_pravac_x;
//                    auto new_y = -m_pravac_y;

//                    m_pravac_x = new_x;
//                    m_pravac_y = new_y;
//                }
//                else {
//                    auto new_x = -m_pravac_x;
//                    auto new_y = m_pravac_y;

//                    m_pravac_x = new_x;
//                    m_pravac_y = new_y;
//                }
//            }
//        }


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


