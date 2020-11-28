#include "code/include/World.hpp"
#include "code/include/Tank.hpp"
#include "code/include/Wall.hpp"
#include "code/include/rocket_1.hpp"

World::World(QObject *parent){

    // Inicijalizacija scene i pogleda

    m_scene = new QGraphicsScene();
    m_scene->setSceneRect(0,0,1280,720);

    m_view = new QGraphicsView();
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setRenderHint(QPainter::Antialiasing);

    m_view->setScene(m_scene);
}
World::~World(){
    delete m_scene;
    delete m_view;
}

void World::show(){

    m_view->setWindowTitle("Tank Attack");
    m_view->setFixedSize(1280,720);
    m_view->show();
}

void World::main_menu(){

    m_scene->clear();
    m_view->setBackgroundBrush(QPixmap(":/resources/images/rsz_tank_background_2.png"));


    QPushButton *bstart = new QPushButton(QString("START GAME"));
    bstart->setFixedWidth(300);
    bstart->setFixedHeight(100);
    bstart->move(m_scene->width()/2 - bstart->rect().width()/2,250);
    bstart->setStyleSheet("background-color: grey");
    m_scene->addWidget(bstart);
    QObject::connect(bstart, SIGNAL (released()), this, SLOT (start()), Qt::QueuedConnection);


    QPushButton *bquit = new QPushButton(QString("QUIT GAME"));
    bquit->setFixedWidth(300);
    bquit->setFixedHeight(100);
    bquit->move(m_scene->width()/2 - bstart->rect().width()/2, 500);
    bquit->setStyleSheet("background-color: grey");
    m_scene->addWidget(bquit);
    QObject::connect(bquit, SIGNAL (released()), this, SLOT (quit()),Qt::QueuedConnection);
    //verovatno i ovde mora ovaj QueuedCOnnection kontam
}

void World::start(){

//    for (size_t i = 0, n = scene->items().size(); i<n; i++){
//        scene->removeItem(scene->items()[0]);
//    }

    m_scene->clear();

    m_view->setBackgroundBrush(Qt::black);
    m_view->setDragMode(QGraphicsView::ScrollHandDrag);

    //Dva tenka jedan nasuprot drugom
    Tank *t1 = new Tank(Qt::red, 200, 400);
    Tank *t2 = new Tank(Qt::blue, 1200, 400);

    //Namestam da tenk1 moze reagovat na stisak tastature
    t1->setFlag(QGraphicsItem::ItemIsFocusable);
    t1->setFocus();
    //##################################################

    //Dva zida koja predstavljaju kako bi mogao da
    //izgleda 1 hodnik na mapi
    Wall *w1 = new Wall(240, 440, 6, 960);
    Wall *w2 = new Wall(240, 380, 6, 960);

    m_scene->addItem(t1);
    m_scene->addItem(t2);
    m_scene->addItem(w1);
    m_scene->addItem(w2);

    qDebug() << "we are here";
}

void World::quit(){
      QApplication::exit(); //mislim da je ovo pravilnije
//    QApplication::closeAllWindows();
}
