#include "code/include/Rocket.hpp"
#include "code/include/World.hpp"
#include <QDebug>
#include <cmath>

int Rocket::rakete_tenka_0;
int Rocket::rakete_tenka_1;

//u rocket power ce se prosledjivati enum u zavisnoti koju jacinu poseduje tenk
Rocket::Rocket(float x, float y, float r, const Rocket_type rocket_type,int id, int x_v, int y_v, qreal rot)
    :m_x(x),m_y(y),m_r(r),m_rocket_type(rocket_type),m_id(id),m_pravac_x(x_v),m_pravac_y(y_v),m_rotation(rot)
{

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


    //dodat nezavisan timer za pomeranje rakete-to Moze!
    QTimer *timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(move()));
    timer->start(30); //move se poziva na svakih 30ms


}
int Rocket::type() const{
    return 3;
}

void Rocket::move()
{
    //Menjamo poziciju i prava kretanja rakete
    if(World::world_pause){
        return;
    }
    m_x -= m_pravac_x;
    m_y -= m_pravac_y;
    setPos(m_x, m_y);


    //lista svih objekata koji imaju koliziju sa rocketom
    QList<QGraphicsItem *> colliding_items = collidingItems();

    if(colliding_items.size() == 2)
    { // za slucaj da udari u dva zida na spoju -> odbija se od onog u kojeg je prvo udarila
         if (typeid(*(colliding_items[0])) == typeid(Wall) &&
             typeid(*(colliding_items[1])) == typeid(Wall))
         {
             Wall *w  = qgraphicsitem_cast<Wall*>(colliding_items[0]);

             if(w->isVertical()) {
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
    }

    for (int i = 0; i < colliding_items.size(); i++)
    {
        //ako raketa udara u zid
        if (typeid(*(colliding_items[i])) == typeid(Wall)){
               Wall *w  = qgraphicsitem_cast<Wall*>(colliding_items.first());
               if(w->isVertical()) {
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
        //ako raketa udara u tenk
        else if(typeid(*(colliding_items[i])) == typeid(Tank)){
              Tank *t = qgraphicsitem_cast<Tank*>(colliding_items[i]);
              t->decrease_health(this->m_rocket_power);
              if (t->get_current_health() <= 0){
                  t->destroy();
              }

              delete this;
              //return je neophodan jer ce doci od segfault---dole opet pristupamo raketi
              //koja je vec obrisana!
              return;
        }
    }
/*

    if (!scene()->collidingItems(this).isEmpty()) {
//        auto tmp_x = m_x;
//        auto tmp_y = m_y;

        if(scene()->collidingItems(this).size() == 1) {
            //0 je id elementa Wall
            if (scene()->collidingItems(this).first()->type() == 0) {
                Wall *w  = qgraphicsitem_cast<Wall*>(scene()->collidingItems(this).first());

                if(w->isVertical()) {
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
            if (scene()->collidingItems(this).first()->type() == 1){
                //1 je id elementa Tank
                Tank *t = qgraphicsitem_cast<Tank*>(scene()->collidingItems(this).first());
                t->destroy();
                delete this;
            }
        }
        else if(scene()->collidingItems(this).size() == 2){
            // za slucaj da udari u dva zida na spoju -> odbija se od onog u kojeg je prvo udarila
            if (scene()->collidingItems(this)[0]->type() == 0 &&
                scene()->collidingItems(this)[1]->type() == 0){
                Wall *w  = qgraphicsitem_cast<Wall*>(scene()->collidingItems(this).first());

                if(w->isVertical()) {
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
        }
    }

*/


   //rakete je potrebno obrisati ako su vec dugo u igri
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
QRectF Rocket::boundingRect() const
{
//    prodiskutovati o ovome
//    return QRectF(m_x-m_r,m_y-m_r,2*m_r,2*m_r);
    return QRectF(-0.5, -0.5, m_r + 1, m_r + 1);
}
void Rocket::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

//    painter->setBrush(m_boja);
//    painter->drawEllipse(m_x,m_y,m_r,m_r);

    painter->setBrush(Qt::yellow);
    painter->drawEllipse(0, 0, m_r, m_r);
}
