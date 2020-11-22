#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QPixmap>
#include <QString>
#include <QGraphicsTextItem>
#include <iostream>
#include <vector>
#include <stack>
#include "code/include/button.hpp"
#include "code/include/Wall.hpp"
#include "code/include/Tank.hpp"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    //prvo kreiramo scenu
    QGraphicsScene scene;
    scene.setSceneRect(0,0,1400,1000);

    QGraphicsView view(&scene);

    //krairamo neki rectItem koji cemo da stavimo na scenu---kasnije ce ovi biti button
//    QGraphicsRectItem *rect = new QGraphicsRectItem();
//    rect->setRect(0,0,100,100);

//    scene->addItem(rect);

    //sada moramo napraviti view da bismo prikazali scenu
   /* QGraphicsView *view = new QGraphicsView(scene);
    view->setWindowTitle("TankAttack");
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(1600,1000);
    view->setBackgroundBrush(QPixmap(":/resources/images/tank_backround.jpg"));


    QGraphicsTextItem *game_name = new QGraphicsTextItem(QString("TANK ATTACK"));
    game_name->setPos(scene->width()/2 - game_name->boundingRect().width()/2,
                      100);
    game_name->setTextWidth(3000); //ova linija mi ne radi ne znam sto!
//    std::cout << game_name->textWidth() << std::endl;
    game_name->setDefaultTextColor(Qt::black);

    view->scene()->addItem(game_name);


    Button *start_button = new Button(QString("START"),100,300);
    start_button->setPos(scene->width()/2 - start_button->boundingRect().width()/2,
                250);
    view->scene()->addItem(start_button);
    */


    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setRenderHint(QPainter::Antialiasing);
    //boja pozadine
    view.setBackgroundBrush(Qt::black);
    view.setDragMode(QGraphicsView::ScrollHandDrag);

    //Dva tenka jedan nasuprot drugom
    Tank *t1 = new Tank(Qt::red, 200, 400);
    Tank *t2 = new Tank(Qt::blue, 1200, 400);

    //Dva zida koja predstavljaju kako bi mogao da
    //izgleda 1 hodnik na mapi
    Wall *w1 = new Wall(240, 440, 6, 960);
    Wall *w2 = new Wall(240, 380, 6, 960);

    scene.addItem(t1);
    scene.addItem(t2);
    scene.addItem(w1);
    scene.addItem(w2);

    view.setWindowTitle("TankAttack");
    view.resize(1300,900);
    view.show();



    return a.exec();
}
