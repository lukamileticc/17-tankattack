#include "code/include/World.hpp"
#include "code/include/Tank.hpp"
#include "code/include/Wall.hpp"
#include "code/include/Input.hpp"
#include "code/include/Map.hpp"
#include <iostream>
#include <QPushButton>
#include <QObject>
#include <iostream>
#include <QBrush>
#include <QApplication>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QLabel>
#include <QVector>
#include <QFile>
#include <exception>
#include <fstream>

World::World(QObject *parent)
    :QObject(parent)
{

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
    //treba editovati destrkutor posto imamo gomilu pokazivaca u klasi!
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

    QPushButton *bstart = new QPushButton(QString("START GAME"));
    bstart->setFixedWidth(300);
    bstart->setFixedHeight(100);
    bstart->move(scene->width()/2 - bstart->rect().width()/2,250);
    bstart->setStyleSheet(style_for_buttons);
    scene->addWidget(bstart);
    QObject::connect(bstart, SIGNAL (released()), this, SLOT (input_players_names()), Qt::QueuedConnection);

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

    view->setBackgroundBrush(QPixmap(":/resources/images/images.jpg"));
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    QPushButton *bback = new QPushButton(QString("Back"));
    bback->setFixedWidth(200);
    bback->setFixedHeight(66);
    bback->move(100, 600);
    scene->addWidget(bback);
    bback->setStyleSheet("QPushButton"
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
                         "}");
    QObject::connect(bback, SIGNAL (released()), this, SLOT (main_menu()),Qt::QueuedConnection);

    //citamo prethodne borbe iz funkcije
     QVector<QString> *previous_battles;
    try{
        previous_battles = read_previous_battles(":/resources/files/istorija_borbi.txt");
    }
    catch(QString e)
    {
       qDebug() << e;
       exit(EXIT_FAILURE);
    }

     QString string="                TOP 10 BATTLES :                 \n";
     QString lista_parova;
     lista_parova.append("\n");
     for(const auto& battle : *previous_battles)
        lista_parova.append("\n").append(battle);

//    QString lista_parova = "\t\t\t1.  tank1 Name1 score1 - score2 tank2 Name2\n"
//                           "\t\t\t2.  tank1 Name1 score1 - score2 tank2 Name2\n"
//                           "\t\t\t3.\n"
//                           "\t\t\t4.\n"
//                           "\t\t\t5.\n"
//                           "\t\t\t6.\n"
//                           "\t\t\t7.\n"
//                           "\t\t\t8.\n"
//                           "\t\t\t9.\n"
//                           "\t\t\t10.\n";
    QFont font("Comic Sans MS", 40, QFont::Bold);
    QFont font_lista("Comic Sans MS", 26);
    font_lista.setItalic(true);
    scene->addText(string, font);
    scene->addText(lista_parova,font_lista);



    delete previous_battles;
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
    QGraphicsTextItem *text = scene->addText(message+" won!", font);
    text->setPos(450, 100);
    text->setDefaultTextColor(QColor("white"));


    QString score_text;
    score_text.reserve(100);
    score_text.append("\n            Score\n").append(t1->get_name()).append(" ").append(QString::number (m_score_t1)).append(" : ").append(QString::number (m_score_t2)).append(" ").append(t2->get_name());
    QGraphicsTextItem *score = scene->addText(score_text,font);
    score->setPos(360,250);
    score->setDefaultTextColor(QColor("white"));

    //kada se zavrsi poslednja runda poziiva se ova funksija da bi azurirala istoriju borbi
    write_the_last_battle("../17-tankattack/code/res/istorija_borbi.txt");

}
void World::write_the_last_battle(const char *file)
{
    //citamo prethodne borbe iz funkcije
     QVector<QString> *previous_battles;
    try{
        previous_battles = read_previous_battles(file);
    }
    catch(QString e)
    {
       qDebug() << e;
       exit(EXIT_FAILURE);
    }

    //izbacam 10. brobu!
    if (previous_battles->length()==10)
        previous_battles->pop_back();
    //ubacam na pocetak poslednju borbu koja se desila
    QString last_battle;
    last_battle.append("                    Red Tank ")
               .append(ime_prvog_tenka).append(" ")
               .append(QString::number(m_score_t1)).append(" ")
               .append("- ")
               .append(QString::number(m_score_t2)).append(" ")
               .append(ime_drugog_tenka).append(" ")
               .append("Blue Tank\n");
    //ovde moze da se doda sa leve i desne strane recimo crvena i plava kockica da ne pise redtank
    //i blue tank --- predlog kad budemo dodavali teksture
    previous_battles->push_front(last_battle);

    for(const auto& battle : *previous_battles)
        qDebug() << battle;

    //editujemo file istorija borbi,odnosno pisemo u njega ceo vektor
//    QFile output_file(file);
//    if(output_file.open(QIODevice::ReadWrite))
//    {
//        QTextStream out(&output_file);
//        for(const auto& battle : *previous_battles)
//            out << battle;
//    }

//    output_file.close();
    std::ofstream myfile;
         myfile.open(file,std::ofstream::trunc);
         if (myfile.is_open()){
         for(const auto& battle : *previous_battles)
                   myfile << battle.toStdString();
         myfile.close();
         }
    delete previous_battles;
}
QVector<QString>* World::read_previous_battles(const char *file)
{
    //citam poslednjih 10 rundi iz file-a istorija brobi i smestam ih u vektor

    QFile input_file(file);
    //ako nismo uspeli da procitamo file , podizemo exception
    if(!input_file.open(QIODevice::ReadOnly))
        throw QString("Open file failed");

    //pravim vektor gde postavljam isotriju brobi
    QVector<QString> *battle_history = new QVector<QString>;
    battle_history->resize(0);

    QTextStream in(&input_file);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        line.append("\n");
        battle_history->push_back(line);
    }

    input_file.close();

//    for(const auto& battle : *battle_history)
//        qDebug() << battle;

    return battle_history;
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

    info_string_t1.append(t1->get_name()).append("\nScore: ").append(QString::number(t1->get_score()))
                  .append("\nHealth: ").append(QString::number(t1->get_current_health()));

    info_t1 = scene->addText(info_string_t1,font);
    info_t1->setPos(8,-35);
    info_t1->setDefaultTextColor("red");
   // info_t1->setPlainText(info_string_t1);

    info_string_t2.append(t2->get_name()).append("\nScore: ").append(QString::number(t2->get_score()))
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
            end_of_round(t2->get_name());
        }
        // t2 win
        //end of round
    }
    else if(t2->is_destroyed()){
        m_left_round_time += 1;
        if (m_left_round_time > 150){
            m_score_t1 += 1;
            t1->set_score(m_score_t1);
            end_of_round(t1->get_name());
        }
        // t1 win
        // end of round
    }



}

void World::start(){
    m_started=1;
    m_in_game=1;


    scene->clear();

    view->setBackgroundBrush(Qt::black);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setFixedSize(1271, 813);


    Input *input = new Input();

    this->t1 = new Tank(0,Qt::red, 200, 400, input);
    this->t2 = new Tank(1,Qt::blue, 1200, 400, input);
    t1->set_name(this->ime_prvog_tenka);
    t2->set_name(this->ime_drugog_tenka);

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

    qDebug() << "helloooo";
    qDebug() << "Ime prvog tenka: " << t1->get_name();
    qDebug() << "Ime drugog tenka: " << t2->get_name();

}
void World::input_players_names()
{
    scene->clear();
    view->setBackgroundBrush(QPixmap(":/resources/images/images.jpg"));
    view->setDragMode(QGraphicsView::ScrollHandDrag);

    QString style_for_button ="QPushButton"
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
                              "}";;

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
    QGraphicsTextItem *text1 =  new QGraphicsTextItem(QString("RED PLAYER NAME"));
    text1->setDefaultTextColor(Qt::red);
    text1->setFont(QFont("Comic Sans MS", 20, QFont::Bold));
    text1->setPos(250,250);
    scene->addItem(text1);

    QGraphicsTextItem *text2 =  new QGraphicsTextItem(QString("BLUE PLAYER NAME"));
    text2->setDefaultTextColor(Qt::blue);
    text2->setFont(QFont("Comic Sans MS", 20, QFont::Bold));
    text2->setPos(250,350);
    scene->addItem(text2);


    //line edit za unos imena igraca
    line1 = new QLineEdit("Enter red tank name");
    line1->setGeometry(600,250,310,50);
    line1->setClearButtonEnabled(true);
    QFont font(QString("Ubuntu"),16);
    font.setItalic(true);
    line1->setFont(font);
    scene->addWidget(line1);


    line2= new QLineEdit("Enter blue tank name");
    line2->setGeometry(600,350,310,50);
    line2->setClearButtonEnabled(true);
    line2->setFont(font);
    scene->addWidget(line2);


    QObject::connect(line1,SIGNAL(textChanged(QString)),this,SLOT(change_name_of_first_tank()),Qt::QueuedConnection);
    QObject::connect(line2,SIGNAL(textChanged(QString)),this,SLOT(change_name_of_second_tank()),Qt::QueuedConnection);
//    this->ime_prvog_tenka = line1->text();
//    this->ime_drugog_tenka = line2->text();

}
void World::change_name_of_first_tank()
{
    this->ime_prvog_tenka = line1->text();
}
void World::change_name_of_second_tank()
{
   this->ime_drugog_tenka = line2->text();
}

void World::quit()
{
      QApplication::exit(); //mislim da je ovo pravilnije
//    QApplication::closeAllWindows();
}
