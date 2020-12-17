#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QPixmap>
#include <QString>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include "code/include/button.hpp"
#include "code/include/Wall.hpp"
#include "code/include/Tank.hpp"
#include "code/include/World.hpp"
#include "code/include/Server.hpp"
#include "../ServerWindow.hpp"
#include "code/ClientWindow.hpp"
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);


    ClientWindow cw;
    cw.show();

    World *world = new World();



    //ServerWindow sw;
    //sw.show();


    world->show();

    world->main_menu();



    /*
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
    */

    return a.exec();
}
