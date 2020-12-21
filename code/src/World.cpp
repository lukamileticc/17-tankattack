#include "code/include/World.hpp"
#include "code/include/Tank.hpp"
#include "code/include/Wall.hpp"
#include "code/include/Input.hpp"
#include "code/include/Map.hpp"
#include "code/include/Rocket.hpp"
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
#include <random>

bool World::world_pause;

QPushButton* World::make_button(QString name)
{
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


    QPushButton *button = new QPushButton(name);
    button->setStyleSheet(style_for_buttons);
    scene->addWidget(button);

    QObject::connect(button, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);

    return button;
}
void World::button_clicked()
{
    QMediaPlayer *button_sound = new QMediaPlayer();
    button_sound->setMedia(QUrl("qrc:/resources/sounds/button_click.wav"));
    button_sound->setVolume(20);
    button_sound->play();
}

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

    //inicijalizujem pozadinsku muziku
    music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/resources/sounds/game_song.mp3"));
    music->setVolume(20);
    music->play();
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


    QPushButton *bstart = make_button("START GAME");
    bstart->setFixedWidth(300);
    bstart->setFixedHeight(100);
    bstart->move(scene->width()/2 - bstart->rect().width()/2,250);
    QObject::connect(bstart, SIGNAL (released()), this, SLOT (input_players_names()), Qt::QueuedConnection);

    if (m_started==1){
        QPushButton *bcontinue = make_button("CONTINUE GAME");

        bcontinue->setFixedWidth(300);
        bcontinue->setFixedHeight(100);

        bcontinue->move(scene->width()/2 - bstart->rect().width()/2,625);
        QObject::connect(bcontinue, SIGNAL (released()), this, SLOT (start()), Qt::QueuedConnection);
    }

    //Button za prikaz prethodnih scoreova
    QPushButton *bbattle = make_button("BATTLES");
    bbattle->setFixedWidth(300);
    bbattle->setFixedHeight(100);
    bbattle->move(scene->width()/2 - bbattle->rect().width()/2,375);
    QObject::connect(bbattle, SIGNAL (released()), this, SLOT (show_battles()), Qt::QueuedConnection);


    //Button za izlaz
    QPushButton *bquit = make_button("  QUIT   GAME");
    bquit->setFixedWidth(300);
    bquit->setFixedHeight(100);
    bquit->move(scene->width()/2 - bquit->rect().width()/2, 500);
    QObject::connect(bquit, SIGNAL (released()), this, SLOT (quit()));


}


void World::show_battles(){
    scene->clear();

    view->setBackgroundBrush(QPixmap(":/resources/images/input.png"));
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    QPushButton *bback = make_button("Back");
    bback->setFixedWidth(200);
    bback->setFixedHeight(66);
    bback->move(980, 600);
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

     QString string="                 LAST 10 BATTLES :                 \n";
     QString lista_parova;
     lista_parova.append("\n\n");
     int i = 1;
     QString tab = "                                                ";
     for(const auto& battle : *previous_battles)
        lista_parova.append("\n").append(tab)
                .append(QString::number(i++)).append(". ")
                .append(battle);


    QFont font("Comic Sans MS", 40, QFont::Bold);
    QFont font_lista("Comic Sans MS", 18);
    QGraphicsTextItem *text = scene->addText(string, font);
    QGraphicsTextItem *text_list = scene->addText(lista_parova,font_lista);
    text->setDefaultTextColor(QColor("white"));
    text_list->setDefaultTextColor(QColor("white"));


    delete previous_battles;
}

void World::end_of_round(QString message){
    //show_tank_info();
    //std::cout << message << std::endl;
    m_ended_round = true;
    QFont font;
    font.setBold(true);
    font.setPointSize(50);
//    scene->clear();
//    view->setBackgroundBrush(Qt::black);
//    view->setDragMode(QGraphicsView::ScrollHandDrag);
//    view->setFixedSize(1271, 813);

//    QLabel *label = new QLabel();
//    label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
//    label->setText(message);
//    label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    QGraphicsTextItem *text = scene->addText(message + " won!", font);
    int x_position = 640 - text->boundingRect().width()/2;
    text->setPos(x_position, 100);
    text->setDefaultTextColor(QColor("white"));

    //"Score" ispis
    QString score1("Score:");
    QGraphicsTextItem *score2 = scene->addText(score1,font);
    int x_pos = 640 - score2->boundingRect().width()/2;
    int y_pos = 300;
    score2->setPos(x_pos,y_pos);
    score2->setDefaultTextColor(QColor("white"));
    //"1:0" rezultat ispis
    QString score3;
    score3.reserve(100);
    score3.append(QString::number (m_score_t1)).append(" : ").append(QString::number (m_score_t2));
    QGraphicsTextItem *score4 = scene->addText(score3,font);
    int x_pos1 = 640 - score4->boundingRect().width()/2;
    int y_pos1 = 400;
    score4->setPos(x_pos1,y_pos1);
    score4->setDefaultTextColor(QColor("white"));
    //ime prvog -- ispis
    QGraphicsTextItem *name1 = scene->addText(ime_prvog_tenka,font);
    int x = 640 - score4->boundingRect().width()/2 - name1->boundingRect().width() - 5;
    int y = 400;
    name1->setPos(x,y);
    name1->setDefaultTextColor(QColor("white"));
    //ime drugog -- ispis
    QGraphicsTextItem *name2 = scene->addText(ime_drugog_tenka,font);
    name2->setPos(640 + score4->boundingRect().width()/2 + 5,y);
    name2->setDefaultTextColor(QColor("white"));


    //Button za next battle
    QPushButton *bnext = make_button("Next Battle");
    bnext->setFixedWidth(200);
    bnext->setFixedHeight(66);
    bnext->move(980,600);
    QObject::connect(bnext, SIGNAL (released()), this, SLOT (start()), Qt::QueuedConnection);


    //Button za quit_game
    QPushButton *bquit =make_button("Quit Game");
    bquit->setFixedWidth(200);
    bquit->setFixedHeight(66);
    bquit->move(100, 600);
    QObject::connect(bquit, SIGNAL (released()), this, SLOT (quit()),Qt::QueuedConnection);




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
    last_battle.append(ime_prvog_tenka).append(" ")
               .append(QString::number(m_score_t1)).append(" ")
               .append("- ")
               .append(QString::number(m_score_t2)).append(" ")
               .append(ime_drugog_tenka).append("\n");

    //ovde moze da se doda sa leve i desne strane recimo crvena i plava kockica da ne pise redtank
    //i blue tank --- predlog kad budemo dodavali teksture
    previous_battles->push_front(last_battle);

    for(const auto& battle : *previous_battles)
        qDebug() << battle;

    //editujemo file istorija borbi,odnosno pisemo u njega ceo vektor

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

void World::pause(){
    if(m_showed_pause){
        scene->removeItem(info_pause);
    }
    m_showed_pause = true;
    QFont font;
    font.setBold(true);
    font.setPointSize(40);
    QString pause_string;
    pause_string.reserve(50);
    pause_string.append("Pause! Press P to continue");

    info_pause = scene->addText(pause_string,font);
    int x_position = 640 - info_pause->boundingRect().width()/2;
    info_pause->setPos(x_position, 300);
    info_pause->setDefaultTextColor(QColor("white"));
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

    info_string_t1.append(t1->get_name()).append("\nScore: ").append(QString::number(m_score_t1))
                  .append("\nHealth: ").append(QString::number(t1->get_current_health()));

    info_t1 = scene->addText(info_string_t1,font);
    info_t1->setPos(8,-35);
    info_t1->setDefaultTextColor("red");
   // info_t1->setPlainText(info_string_t1);

    info_string_t2.append(t2->get_name()).append("\nScore: ").append(QString::number(m_score_t2))
                  .append("\nHealth: ").append(QString::number(t2->get_current_health()));

    info_t2 = scene->addText(info_string_t2,font);
    info_t2->setPos(1168,-35);
    info_t2->setDefaultTextColor("blue");
    //info_t2->setPlainText(info_string_t2);

}

void World::rounds(){
    if(m_ended_round)
        return;

    if(t1->is_pause() || t2->is_pause()){
        world_pause = true;
        pause();
        return;
    }
    else{
        world_pause = false;
        scene->removeItem(info_pause);
    }

    show_tank_info();
    if(t1->is_destroyed() && t2->is_destroyed()){
        // nobody win
        end_of_round("Nobody");
    }
    else if (t1->is_destroyed()){

        m_left_round_time += 1;
        qDebug() << m_left_round_time;
        if(m_left_round_time == 1) {
            t2->set_end_of_round();
        }
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

        if(m_left_round_time == 1) {
            t1->set_end_of_round();
        }
        if (m_left_round_time > 150){
            m_score_t1 += 1;
            t1->set_score(m_score_t1);
            end_of_round(t1->get_name());
        }
        // t1 win
        // end of round
    }



}

int rand_int(int nMin, int nMax)
{
    return nMin + (int)((double)rand() / (RAND_MAX+1) * (nMax-nMin+1));
}

void World::load_map(){

    //Mapa otvara odgovarajuci fajl
    //Pravi zidove i vraca te zidove kako bi ih postavili na scenu
    char mapa[] = ":/resources/files/mapa_.txt";

    // u zavisnosti od odabrane mape char mapa[] "../mapa_.txt"
    // se menja u "../mapax.txt" gde je x random broj dobijen funkcijom RandU

    int random_integer = rand_int(1,1);
    /* PROVERA DA LI JE NOVA MAPA RAZLICITA OD PRETHODNE, NEMA SMISLA SAD JER IMAMO SAMO JEDNU MAPU
    while(random_integer == m_last_map){
        random_integer = rand_int(1,1);
        qDebug() << random_integer;
    }
    */

    m_last_map = random_integer;

    char number_of_map[2];
    std::sprintf(number_of_map, "%d", random_integer);
    mapa[22] = number_of_map[0];
    qDebug() << mapa;

    Map *m1 = new Map(mapa);
    std::vector<Wall*>walls = m1->getWalls();

    for(auto w: walls)
        scene->addItem(w);

}

void World::start(){
    music->pause();
    m_ended_round = false;
    m_showed_info = false;
    World::world_pause = false;
    Rocket::rakete_tenka_0 = 0;
    Rocket::rakete_tenka_1 = 0;
    m_left_round_time = 0;
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

    load_map();

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
    view->setBackgroundBrush(QPixmap(":/resources/images/input.png"));
    view->setDragMode(QGraphicsView::ScrollHandDrag);


    //Button za start_battle
    QPushButton *bbattle =make_button("Start Battle");
    bbattle->setFixedWidth(200);
    bbattle->setFixedHeight(66);
    bbattle->move(980,600);
    QObject::connect(bbattle,SIGNAL(released()),this,SLOT(start()),Qt::QueuedConnection);


    //Button za back
    QPushButton *bback =make_button("Back");
    bback->setFixedWidth(200);
    bback->setFixedHeight(66);
    bback->move(100, 600);
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
