#include "code/include/World.hpp"
#include "code/include/Tank.hpp"
#include "code/include/Wall.hpp"
#include "code/include/Input.hpp"
#include "code/include/Map.hpp"
#include <QPushButton>
#include <QObject>
#include <iostream>
#include <QBrush>
#include <QApplication>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QLabel>
#include <QLineEdit>

World::World(QObject *parent){
    Q_UNUSED(parent);

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
    view->setFixedSize(1280, 720);
    view->show();
}

void World::main_menu(){
    m_in_game=0;
    scene->clear();
    view->setBackgroundBrush(QPixmap(":/resources/images/rsz_tank_background_2.png"));

    QString style_for_buttons = "QPushButton"
                                "{"
                                 "background-color: teal;"
                                 "border-style: outset;"
                                 "border-width: 2.5px;"
                                 "border-radius: 10px;"
                                 "border-color: beige;"
                                 "font: bold 16px;"
                                 "padding: 6px;"
                                "}"
                                "QPushButton::hover"
                                "{"
                                "background-color: #00CDFF"
                                "}";

    //QPushButton *bstart = new QPushButton(QString(" START GAME   "));

    QPushButton *bstart = new QPushButton(QString("START GAME"));

    bstart->setFixedWidth(300);
    bstart->setFixedHeight(100);

    bstart->move(scene->width()/2 - bstart->rect().width()/2,250);
    bstart->setStyleSheet(style_for_buttons);
    scene->addWidget(bstart);
    QObject::connect(bstart, SIGNAL (released()), this, SLOT (input_players_names()), Qt::QueuedConnection);

    bstart->setStyleSheet("background-color: teal;"
                        "border-style: outset;"
                         "border-width: 2.5px;"
                         "border-radius: 10px;"
                         "border-color: beige;"
                         "font: bold 16px;"
                         "padding: 6px;");

    scene->addWidget(bstart);

    if (m_started==1){
        QPushButton *bcontinue = new QPushButton(QString("CONTINUE GAME"));

        bcontinue->setFixedWidth(300);
        bcontinue->setFixedHeight(100);

        bcontinue->move(scene->width()/2 - bstart->rect().width()/2,625);
        bcontinue->setStyleSheet(style_for_buttons);
        scene->addWidget(bcontinue);
        QObject::connect(bcontinue, SIGNAL (released()), this, SLOT (start()), Qt::QueuedConnection);
    }

    //Button za prikaz prethodnih scoreova
    QPushButton *bbattle = new QPushButton(QString("BATTLES"));

    bbattle->setFixedWidth(300);
    bbattle->setFixedHeight(100);

    bbattle->move(scene->width()/2 - bbattle->rect().width()/2,375);
    bbattle->setStyleSheet(style_for_buttons);
    scene->addWidget(bbattle);
    QObject::connect(bbattle, SIGNAL (released()), this, SLOT (show_battles()), Qt::QueuedConnection);

    //Button za izlaz
    QPushButton *bquit = new QPushButton(QString("  QUIT   GAME"));
    bquit->setFixedWidth(300);
    bquit->setFixedHeight(100);

    bquit->move(scene->width()/2 - bquit->rect().width()/2, 500);
    bquit->setStyleSheet(style_for_buttons);
    scene->addWidget(bquit);
    QObject::connect(bquit, SIGNAL (released()), this, SLOT (quit()));


}


void World::show_battles(){
    scene->clear();

    view->setBackgroundBrush(Qt::blue);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    QPushButton *bback = new QPushButton(QString("Back"));
    bback->setFixedWidth(100);
    bback->setFixedHeight(33);
    bback->move(1100, 600);
    scene->addWidget(bback);
    QObject::connect(bback, SIGNAL (released()), this, SLOT (main_menu()),Qt::QueuedConnection);

    QString string="                TOP 10 BATTLES :                 \n";
    QString lista_parova = "\n\n\t\t\t1.  tank1 Name1 score1 - score2 tank2 Name2\n"
                           "\t\t\t2.  tank1 Name1 score1 - score2 tank2 Name2\n"
                           "\t\t\t3.\n"
                           "\t\t\t4.\n"
                           "\t\t\t5.\n"
                           "\t\t\t6.\n"
                           "\t\t\t7.\n"
                           "\t\t\t8.\n"
                           "\t\t\t9.\n"
                           "\t\t\t10.\n";
    QFont font("Comic Sans MS", 40, QFont::Bold);
    QFont font_lista("Comic Sans MS", 26);
    font_lista.setItalic(true);
    scene->addText(string, font);
    scene->addText(lista_parova,font_lista);


}

void World::end_of_round(QString message){
    //show_tank_info();
    //std::cout << message << std::endl;
    m_ended_round = true;
    QFont font;
    font.setBold(true);
    font.setPointSize(50);
    scene->clear();
    view->setBackgroundBrush(Qt::black);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setFixedSize(1271, 813);

//    QLabel *label = new QLabel();
//    label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
//    label->setText(message);
//    label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    QGraphicsTextItem *text = scene->addText(message, font);
    text->setPos(450, 100);
    text->setDefaultTextColor(QColor("white"));


    QString score_text;
    score_text.reserve(100);
    score_text.append("\n            Score\n").append("Player1 ").append(QString::number (m_score_t1)).append(" : ").append(QString::number (m_score_t2)).append(" Player2");
    QGraphicsTextItem *score = scene->addText(score_text,font);
    score->setPos(360,250);
    score->setDefaultTextColor(QColor("white"));
}

void World::show_tank_info(){
    if (m_showed_info){
        scene->removeItem(info_t1);
        scene->removeItem(info_t2);
    }
    m_showed_info = true;
    QFont font;
    font.setBold(true);
    font.setPointSize(12);

    QString info_string_t1,info_string_t2;
    info_string_t1.reserve(50);
    info_string_t2.reserve(50);

    info_string_t1.append("Player1\n").append("Score: ").append(QString::number(t1->get_score()))
                  .append("\nHealth: ").append(QString::number(t1->get_current_health()));

    info_t1 = scene->addText(info_string_t1,font);
    info_t1->setPos(8,-35);
    info_t1->setDefaultTextColor("red");
   // info_t1->setPlainText(info_string_t1);

    info_string_t2.append("Player2\n").append("Score: ").append(QString::number(t2->get_score()))
                  .append("\nHealth: ").append(QString::number(t2->get_current_health()));

    info_t2 = scene->addText(info_string_t2,font);
    info_t2->setPos(1168,-35);
    info_t2->setDefaultTextColor("blue");
    //info_t2->setPlainText(info_string_t2);

}

void World::rounds(){
    if(m_ended_round)
        return;
    show_tank_info();
    if(t1->is_destroyed() && t2->is_destroyed()){
        // nobody win
        end_of_round("Nobody won!");
    }
    else if (t1->is_destroyed()){

        m_left_round_time += 1;
        if (m_left_round_time > 150){
            m_score_t2 += 1;
            t2->set_score(m_score_t2);
            end_of_round("Player2 won!");
        }
        // t2 win
        //end of round
    }
    else if(t2->is_destroyed()){
        m_left_round_time += 1;
        if (m_left_round_time > 150){
            m_score_t1 += 1;
            t1->set_score(m_score_t1);
            end_of_round("Player1 won!");
        }
        // t1 win
        // end of round
    }



}

void World::start(){
    m_started=1;
    m_in_game=1;

//    for (size_t i = 0, n = scene->items().size(); i<n; i++){
//        scene->removeItem(scene->items()[0]);
//    }

    scene->clear();

    view->setBackgroundBrush(Qt::black);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setFixedSize(1271, 813);


    Input *input = new Input();

    this->t1 = new Tank(0,Qt::red, 200, 400, input);
    this->t2 = new Tank(1,Qt::blue, 1200, 400, input);



    scene->addItem(t1);
    scene->addItem(t2);
    scene->addItem(input);

    input->setFlag(QGraphicsItem::ItemIsFocusable);
    input->setFocus();

    //Mapa otvara odgovarajuci fajl
    //Pravi zidove i vraca te zidove kako bi ih postavili na scenu
    Map *m1 = new Map(":/resources/files/mapa1.txt");
    std::vector<Wall*>walls = m1->getWalls();

    for(auto w: walls)
        scene->addItem(w);

    QObject::connect(input->timer, SIGNAL(timeout()), t1, SLOT(advance()));
    QObject::connect(input->timer, SIGNAL(timeout()), t2, SLOT(advance()));
    QObject::connect(input->timer, SIGNAL(timeout()), this, SLOT(rounds()));
    input->timer->start(33);
    //show_tank_info();

    std::cout << "helloooo" << std::endl;

}
void World::input_players_names()
{
    scene->clear();
    view->setBackgroundBrush(QPixmap(":/resources/images/rsz_tank_background_2.png"));
    view->setDragMode(QGraphicsView::ScrollHandDrag);

    QString style_for_button = "QPushButton{"
                               "background-color: #00F7FF;"
                               "border-style: outset;"
                                "border-width: 2.5px;"
                                "border-color: black;"
                                "font: bold 16px;"
                                "padding: 6px;"
                                "}"
                                "QPushButton::hover{"
                                   "background-color: #0077FF;"
                                "}";

    //Button za start_battle
    QPushButton *bbattle = new QPushButton(QString("Start Battle"));
    bbattle->setFixedWidth(200);
    bbattle->setFixedHeight(66);
    bbattle->move(980,600);
    scene->addWidget(bbattle);
    bbattle->setStyleSheet(style_for_button);
    QObject::connect(bbattle,SIGNAL(released()),this,SLOT(start()),Qt::QueuedConnection);


    //Button za back
    QPushButton *bback = new QPushButton(QString("Back"));
    bback->setFixedWidth(200);
    bback->setFixedHeight(66);
    bback->move(100, 600);
    scene->addWidget(bback);
    bback->setStyleSheet(style_for_button);
    QObject::connect(bback, SIGNAL (released()), this, SLOT (main_menu()),Qt::QueuedConnection);

    //labeli za unos imena igraca
    QGraphicsTextItem *text1 =  new QGraphicsTextItem(QString("BLUE PLAYER NAME"));
    text1->setDefaultTextColor(Qt::blue);
    text1->setFont(QFont("Comic Sans MS", 20, QFont::Bold));
    text1->setPos(250,250);
    scene->addItem(text1);

    QGraphicsTextItem *text2 =  new QGraphicsTextItem(QString("RED PLAYER NAME"));
    text2->setDefaultTextColor(Qt::red);
    text2->setFont(QFont("Comic Sans MS", 20, QFont::Bold));
    text2->setPos(250,350);
    scene->addItem(text2);


    //line edit za unos imena igraca
    QLineEdit *line1 = new QLineEdit("Enter name of blue tank");
    QLineEdit *line2 = new QLineEdit("Enter name of red tank");
    line1->setGeometry(600,250,310,50);
    line2->setGeometry(600,350,310,50);
    line1->setClearButtonEnabled(true);
    line2->setClearButtonEnabled(true);
    QFont font(QString("Ubuntu"),16);
    font.setItalic(true);
    line1->setFont(font);
    line2->setFont(font);
    scene->addWidget(line1);
    scene->addWidget(line2);

}
void World::quit()
{
      QApplication::exit(); //mislim da je ovo pravilnije
//    QApplication::closeAllWindows();
}
