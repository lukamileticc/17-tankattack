#include "code/include/World.hpp"
#include "code/include/Tank.hpp"
#include "code/include/Wall.hpp"
#include <QPushButton>
#include <QObject>
#include <iostream>
#include <QBrush>
#include <QApplication>

World::World(QObject *parent){

    // Inicijalizacija scene i pogleda

    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1400,1000);

    view = new QGraphicsView();
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHint(QPainter::Antialiasing);

    view->setScene(scene);
}

void World::show(){

    view->setWindowTitle("Tank Attack");
    view->setFixedSize(1400,1000);
    view->show();
}

void World::main_menu(){

    scene->clear();
    view->setBackgroundBrush(QPixmap(":/resources/images/tank_background_2.png"));

    QPushButton *bstart = new QPushButton(QString("START GAME"));

    bstart->setFixedWidth(300);
    bstart->setFixedHeight(100);

    bstart->move(scene->width()/2 - bstart->rect().width()/2,250);

    bstart->setStyleSheet("background-color: grey");

    scene->addWidget(bstart);
    QObject::connect(bstart, SIGNAL (released()), this, SLOT (start()));

    QPushButton *bquit = new QPushButton(QString("QUIT GAME"));

    bquit->setFixedWidth(300);
    bquit->setFixedHeight(100);

    bquit->move(scene->width()/2 - bstart->rect().width()/2, 500);

    bquit->setStyleSheet("background-color: grey");

    scene->addWidget(bquit);
    QObject::connect(bquit, SIGNAL (released()), this, SLOT (quit()));

}

void World::start(){


//    for (size_t i = 0, n = scene->items().size(); i<n; i++){
//        scene->items()[i]->setEnabled(false);
//    }

    for (size_t i = 0, n = scene->items().size(); i<n; i++){
        scene->removeItem(scene->items()[0]);
    }

    view->setBackgroundBrush(Qt::black);
    view->setDragMode(QGraphicsView::ScrollHandDrag);

    //Dva tenka jedan nasuprot drugom
    Tank *t1 = new Tank(Qt::red, 200, 400);
    Tank *t2 = new Tank(Qt::blue, 1200, 400);

    //Dva zida koja predstavljaju kako bi mogao da
    //izgleda 1 hodnik na mapi
    Wall *w1 = new Wall(240, 440, 6, 960);
    Wall *w2 = new Wall(240, 380, 6, 960);

    scene->addItem(t1);
    scene->addItem(t2);
    scene->addItem(w1);
    scene->addItem(w2);


    std::cout << "helloooo" << std::endl;

}

void World::quit(){
    QApplication::closeAllWindows();
}
