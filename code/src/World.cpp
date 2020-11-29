#include "code/include/World.hpp"
#include "code/include/Tank.hpp"
#include "code/include/Wall.hpp"
#include "code/include/Map.hpp"

World::World(QObject *parent){

    // Inicijalizacija scene i pogleda

    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1280,720);

    view = new QGraphicsView();
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHint(QPainter::Antialiasing);

    view->setScene(scene);
}
World::~World(){
    delete scene;
    delete view;
}

void World::show(){

    view->setWindowTitle("Tank Attack");
    //view->setFixedSize(1280,720);
    view->show();
}

void World::main_menu(){

    scene->clear();
    view->setBackgroundBrush(QPixmap(":/resources/images/rsz_tank_background_2.png"));


    QPushButton *bstart = new QPushButton(QString("START GAME"));
    bstart->setFixedWidth(300);
    bstart->setFixedHeight(100);
    bstart->move(scene->width()/2 - bstart->rect().width()/2,250);
    bstart->setStyleSheet("background-color: grey");
    scene->addWidget(bstart);
    QObject::connect(bstart, SIGNAL (released()), this, SLOT (start()), Qt::QueuedConnection);


    QPushButton *bquit = new QPushButton(QString("QUIT GAME"));
    bquit->setFixedWidth(300);
    bquit->setFixedHeight(100);
    bquit->move(scene->width()/2 - bstart->rect().width()/2, 500);
    bquit->setStyleSheet("background-color: grey");
    scene->addWidget(bquit);
    QObject::connect(bquit, SIGNAL (released()), this, SLOT (quit()),Qt::QueuedConnection);
    //verovatno i ovde mora ovaj QueuedCOnnection kontam
}

void World::start(){

//    for (size_t i = 0, n = scene->items().size(); i<n; i++){
//        scene->removeItem(scene->items()[0]);
//    }

    scene->clear();

    view->setBackgroundBrush(Qt::black);
    view->setDragMode(QGraphicsView::ScrollHandDrag);

    //TODO: Da menu bude jedne velicine, a kada se klikne
    //na start bude fiksne druge velicine, da bi stala cela mapa
    view->resize(1300, 900);

    //Dva tenka jedan nasuprot drugom
    Tank *t1 = new Tank(Qt::red, 100, 400);
    Tank *t2 = new Tank(Qt::blue, 1200, 400);

    scene->addItem(t1);
    scene->addItem(t2);


    //Mapa otvara odgovarajuci fajl
    //Pravi zidove i vraca te zidove kako bi ih postavili na scenu
    Map *m1 = new Map("mapa1.txt");
    std::vector<Wall*>walls = m1->getWalls();

    for(auto w: walls)
        scene->addItem(w);

    std::cout << "helloooo" << std::endl;

}

void World::quit(){
      QApplication::exit(); //mislim da je ovo pravilnije
//    QApplication::closeAllWindows();
}
