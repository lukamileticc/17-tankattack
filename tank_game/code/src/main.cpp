#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QPixmap>
#include <QString>
#include <QGraphicsTextItem>
#include <iostream>
#include "code/include/button.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //prvo kreiramo scenu
    QGraphicsScene *scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1600,1000);


    //krairamo neki rectItem koji cemo da stavimo na scenu---kasnije ce ovi biti button
//    QGraphicsRectItem *rect = new QGraphicsRectItem();
//    rect->setRect(0,0,100,100);

//    scene->addItem(rect);

    //sada moramo napraviti view da bismo prikazali scenu
    QGraphicsView *view = new QGraphicsView(scene);
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


    view->show();

    return a.exec();
}
