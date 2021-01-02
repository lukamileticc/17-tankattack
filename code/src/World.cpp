#include "code/include/World.hpp"
#include "code/include/Tank.hpp"
#include "code/include/Wall.hpp"
#include "code/include/Input.hpp"
#include "code/include/Map.hpp"
#include "code/include/Rocket.hpp"
#include "code/include/SuperPower.hpp"
#include "code/include/Server.hpp"
#include <QRandomGenerator>
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
bool World::isHosting = false;

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

    return button;
}
void World::button_clicked()
{
    button_sound->play();
}

World::World(QObject *parent)
    :QObject(parent)
{    
    Q_UNUSED(parent);

    // Inicijalizacija scene i pogleda

    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 1150, 768);

    view = new QGraphicsView();
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHint(QPainter::Antialiasing);

    view->setScene(scene);

    //inicijalizujem pozadinsku muziku
    music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/resources/sounds/game_song.mp3"));
    music->setVolume(30);

    button_sound = new QMediaPlayer();
    button_sound->setMedia(QUrl("qrc:/resources/sounds/button_click.wav"));
    button_sound->setVolume(20);

}

World::~World(){
    //UNISTEN TENK SE SAM UNISTAVA , s tim sto na kraju svake runde treba unisti oba tenka
    //jer se u sledece pozivu next_round ona oba opet inicijalizuju
    //TENKOVI SE PRAVILNO OSLOBADJAJU!
    //I input se pravilno oslobadja!

    //muzika se pravilno oslobadja
    delete music;
    //i button sound se ispravno deiicijalizuje
    delete  button_sound;


    //info_pause i info_t2 i info_t1 i game_score se pravilno oslobadjaja
    delete info_pause;
    delete info_t2;
    delete info_t1;
    delete game_score;

//OVO DOLE NE ZNAM DA OSLOBODIM
    //Ove lajnove ne znam da oslobdim u pm
//    delete line2;
//    delete line1;
//    delete warning_name_length;


    //bvolumen nije potrebno oslobadjati ,to za nas radi qt-vrvtno
//  delete bvolumen;

}

void World::show(){

    view->setWindowTitle("Tank Attack");
    view->setFixedSize(1150, 768);
    view->show();
}

void World::main_menu(){

    music->play();
    m_showed_warning = false;
    m_in_game=0;
    scene->clear();
    view->setBackgroundBrush(QPixmap(":/resources/images/rsz_tank_background_2.png"));


    //button za pojacavanje i smanjivanje volumena
    bvolumen = make_button("VOLUMEN");
    bvolumen->move(100,100);
    QPixmap pixmap(":/resources/images/volumen_100.png");
    bvolumen->setIcon(QIcon(pixmap));
    bvolumen->setIconSize(QSize(100,100));
    bvolumen->setCheckable(true);
    //ako pravimo bvolumen onda necemo standardni styleSheet
    bvolumen->setStyleSheet(QString("height:100px;"
                                  "width: 100px;"
                                  "background-color: rgba(255,255,255,0);"
                                  "border: 0px;"));
    bvolumen->setText(QString(""));
    QObject::connect(bvolumen, SIGNAL (released()), this, SLOT (set_volume()), Qt::QueuedConnection);
    QObject::connect(bvolumen, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);


    QPushButton *bstart = make_button("START GAME");
    bstart->setFixedWidth(300);
    bstart->setFixedHeight(100);
    bstart->move(scene->width()/2 - bstart->rect().width()/2,250);
    QObject::connect(bstart, SIGNAL (released()), this, SLOT (input_players_names()), Qt::QueuedConnection);
    QObject::connect(bstart, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);

   
    if (m_started==1){
        QPushButton *bcontinue = make_button("CONTINUE GAME");

        bcontinue->setFixedWidth(300);
        bcontinue->setFixedHeight(100);
        bcontinue->move(scene->width()/2 - bstart->rect().width()/2,625);
        QObject::connect(bcontinue, SIGNAL (released()), this, SLOT (start()), Qt::QueuedConnection);
        QObject::connect(bcontinue, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);
    }

    //Button za prikaz prethodnih scoreova
    QPushButton *bbattle = make_button("BATTLES");
    bbattle->setFixedWidth(300);
    bbattle->setFixedHeight(100);
    bbattle->move(scene->width()/2 - bbattle->rect().width()/2, 500);
    QObject::connect(bbattle, SIGNAL (released()), this, SLOT (show_battles()), Qt::QueuedConnection);
    QObject::connect(bbattle, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);


    //Button za izlaz
    QPushButton *bquit = make_button("  QUIT   GAME");
    bquit->setFixedWidth(300);
    bquit->setFixedHeight(100);
    bquit->move(scene->width()/2 - bquit->rect().width()/2, 625);
    QObject::connect(bquit, SIGNAL (released()), this, SLOT (quit()));
    QObject::connect(bquit, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);

    //Multiplayer
    QPushButton *bmultiplayer = make_button("Multiplayer");
    bmultiplayer->setFixedWidth(300);
    bmultiplayer->setFixedHeight(100);
    bmultiplayer->move(scene->width()/2 - bmultiplayer->rect().width()/2, 375);
    QObject::connect(bmultiplayer, SIGNAL(released()), this, SLOT (multiplayer_menu()), Qt::QueuedConnection);
    QObject::connect(bmultiplayer, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);
}
void World::set_volume()
{
    if(music->volume() == 0)
    {
        QPixmap pixmap(":/resources/images/volumen_100.png");
        bvolumen->setIcon(QIcon(pixmap));
        music->setVolume(30);
    }
    else if(music->volume() == 10)
    {
        QPixmap pixmap(":/resources/images/volumen_0.png");
        bvolumen->setIcon(QIcon(pixmap));
        music->setVolume(0);
    }
    else if(music->volume() == 20)
    {
        QPixmap pixmap(":/resources/images/volumen_33.png");
        bvolumen->setIcon(QIcon(pixmap));
        music->setVolume(10);
    }
    else if(music->volume() == 30)
    {
        QPixmap pixmap(":/resources/images/volumen_66.png");
        bvolumen->setIcon(QIcon(pixmap));
        music->setVolume(20);
    }
}

void World::multiplayer_menu()
{

      isMultiplayer = true;
      scene->clear();
      view->setBackgroundBrush(QPixmap(":/resources/images/rsz_tank_background_2.png"));
      QPushButton *bhost = make_button("Host game");
      bhost->setFixedWidth(300);
      bhost->setFixedHeight(100);
      bhost->move(scene->width()/2 - bhost->rect().width()/2,250);
      QObject::connect(bhost, SIGNAL (released()), this, SLOT (start_server()),Qt::QueuedConnection);
      QObject::connect(bhost, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);



    QPushButton *bfind = make_button("Find game");
    bfind->setFixedWidth(300);
    bfind->setFixedHeight(100);
    bfind->move(scene->width()/2 - bfind->rect().width()/2,375);
    QObject::connect(bfind, SIGNAL (released()), this, SLOT (find_game()),Qt::QueuedConnection);
    QObject::connect(bfind, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);
}

void World::start_server()
{
    qDebug() << "mrk1";
    Server *server = new Server();
    if (!server->listen(QHostAddress::Any, 1967)) {
        qDebug() << "Server hasn't started!";
    }
    else
        qDebug() << "Server has started";


    //server->waitForClients();

    isHosting = true;

    start();
    qDebug() << "mrk";
}

void World::find_game()
{

    start();
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
    QObject::connect(bback, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);

    //citamo prethodne borbe iz funkcije
    QVector<QString> *previous_battles = nullptr;
    try{
        previous_battles = read_previous_battles(":/resources/files/istorija_borbi.txt");
    }
    catch(const QString &e)
    {

       qDebug() << e;
       delete previous_battles;
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


    //brisemo pokazivac na vektor
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

void World::start(){


    music->pause();
    m_ended_round = false;
    m_showed_info = false;
    m_showed_warning = false;
    World::world_pause = false;
    Rocket::rakete_tenka_0 = 0;
    Rocket::rakete_tenka_1 = 0;
    m_left_round_time = 0;
    m_started=1;
    m_in_game=1;

//    for (size_t i = 0, n = scene->items().size(); i<n; i++){
//        scene->removeItem(scene->items()[0]);
//    }

    scene->clear();

    view->setBackgroundBrush(QPixmap(":/resources/images/map_1_background.png"));
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setFixedSize(1150, 768);


    input = new Input();
    QColor host = Qt::blue;
    QColor client = Qt::red;

    if(isHosting)
    {
        host = Qt::red;
        client = Qt::blue;
    }

    this->t1 = new Tank(0,Qt::red, 80, 500, input, host, client);
    this->t2 = new Tank(1,Qt::blue, 1045, 500, input, host, client);
    t1->set_name(this->first_tank_name);
    t2->set_name(this->second_tank_name);
//    SuperPower *sp= new SuperPower("superpower",QRandomGenerator::global()->bounded(1240),QRandomGenerator::global()->bounded(600),30);
//    SuperPower *sp1= new SuperPower("health",QRandomGenerator::global()->bounded(1240),QRandomGenerator::global()->bounded(600),30);
//    SuperPower *sp2= new SuperPower("speed",QRandomGenerator::global()->bounded(1240),QRandomGenerator::global()->bounded(600),30);
//    if(isHosting){
//        t1->setColors();
//        t2->setColors();
//        qDebug() << "adasd";
//    }
    qDebug()<<"ovdeded";

    scene->addItem(t1);
    scene->addItem(t2);
    scene->addItem(input);
//    scene->addItem(sp);
//    scene->addItem(sp1);
//    scene->addItem(sp2);
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
int rand_int(int nMin, int nMax)
{
    //ovde imamo potencijalni owerflow - ispraviti ako moze
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
    std::vector<Wall *> walls = m1->getWalls();


    for(auto w: walls)
        scene->addItem(w);


    //mapa nam vise nije potrebna
    //brisemo je da ne bi doslo do curenja memorije
    delete m1;
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
        //Da se ne bi ispisivalo cant remove from the scene
        //ovde se treba proveriti da li je info_pause na sceni
        //ako jeste onda ga brisi! a ako nije da uopste ne ulazi ovde!
        //scene->removeItem(info_pause);
    }

    show_tank_info();
    if(t1->is_destroyed() && t2->is_destroyed()){
        // nobody win
        end_of_round("Nobody");
        return;
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

        return;
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
        return;
    }

}
void World::pause(){
    if(m_showed_pause){
        scene->removeItem(info_pause);
        //da ne bi curela memorija
        delete info_pause;
    }
    m_showed_pause = true;
    QFont font;
    font.setBold(true);
    font.setPointSize(40);
    QString pause_string;
    pause_string.reserve(50);
    pause_string.append("Pause! Press P to continue");

    //ovde se inicijalizuje info_pause ,znaci ako se vise puta pozove pause() ,ovde ce da curi
    //zato je potrebno gore delete info_pause
    info_pause = scene->addText(pause_string,font);
    int x_position = 640 - info_pause->boundingRect().width()/2;
    info_pause->setPos(x_position, 300);
    info_pause->setDefaultTextColor(QColor("white"));
}
void World::show_tank_info() {

    if (m_showed_info) {
        scene->removeItem(info_t1);
        delete info_t1;
        scene->removeItem(info_t2);
        delete info_t2;
        scene->removeItem(game_score);
        delete game_score;
    }

    m_showed_info = true;

    QFont font1 = QFont("Helvetica", 20, QFont::Bold);

    QFont font2 = QFont("Helvetica", 20, QFont::Bold);

    QString info_string_t1, info_string_t2, game_score_string;
    info_string_t1.reserve(50);
    info_string_t2.reserve(50);
    game_score_string.reserve(50);

    info_string_t1.append(first_tank_name);
    info_t1 = scene->addText(info_string_t1, font1);
    info_t1->setPos(38, 702);
    info_t1->setDefaultTextColor("white");
   // info_t1->setPlainText(info_string_t1);

    info_string_t2.append(second_tank_name);
    info_t2 = scene->addText(info_string_t2, font1);
    info_t2->setPos(763, 702);
    info_t2->setDefaultTextColor("white");
    //info_t2->setPlainText(info_string_t2);

    game_score_string.append(QString::number(m_score_t1)).append(":").append(QString::number(m_score_t2));
    game_score = scene->addText(game_score_string, font2);
    game_score->setPos(550, 702);
    game_score->setDefaultTextColor("white");
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
    QGraphicsTextItem *name1 = scene->addText(first_tank_name,font);
    int x = 640 - score4->boundingRect().width()/2 - name1->boundingRect().width() - 5;
    int y = 400;
    name1->setPos(x,y);
    name1->setDefaultTextColor(QColor("white"));
    //ime drugog -- ispis
    QGraphicsTextItem *name2 = scene->addText(second_tank_name,font);
    name2->setPos(640 + score4->boundingRect().width()/2 + 5,y);
    name2->setDefaultTextColor(QColor("white"));


    //Button za next battle
    QPushButton *bnext = make_button("Next Battle");
    bnext->setFixedWidth(200);
    bnext->setFixedHeight(66);
    bnext->move(980,600);
    QObject::connect(bnext, SIGNAL (released()), this, SLOT (start()), Qt::QueuedConnection);
    QObject::connect(bnext, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);



    //Button za quit_game
    QPushButton *bquit =make_button("Quit Game");
    bquit->setFixedWidth(200);
    bquit->setFixedHeight(66);
    bquit->move(100, 600);
    QObject::connect(bquit, SIGNAL (released()), this, SLOT (quit()),Qt::QueuedConnection);
    QObject::connect(bquit, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);



    //moramo osloboditi oba tenka jer ce u sledecoj rundi da budu opet inicijalizovani
    //tek ovde Brisemo tenkove
    delete t1;
    delete t2;
    //Ovde se brise i input jer ce se kao i tenkovi opet inicijalizovati u sledecoj rundi
    delete input;


    //kada se zavrsi poslednja runda poziiva se ova funksija da bi azurirala istoriju borbi
    write_the_last_battle("../17-tankattack/code/res/istorija_borbi.txt");

    return;

}
void World::write_the_last_battle(const char *file)
{
    //citamo prethodne borbe iz funkcije
     QVector<QString> *previous_battles = nullptr;
    try{
        previous_battles = read_previous_battles(file);
    }
    catch(QString e)
    {
       qDebug() << e;
       delete previous_battles;
       exit(EXIT_FAILURE);
    }

    //izbacam 10. brobu!
    if (previous_battles->length()==10)
        previous_battles->pop_back();
    //ubacam na pocetak poslednju borbu koja se desila
    QString last_battle;
    last_battle.append(first_tank_name).append(" ")
               .append(QString::number(m_score_t1)).append(" ")
               .append("- ")
               .append(QString::number(m_score_t2)).append(" ")
               .append(second_tank_name).append("\n");

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
void World::input_players_names()
{
    //svaki put kad opet udjemo u ovaj_menu treba da brisemo pokazivace da ne bi curela memorija

    scene->clear();
    view->setBackgroundBrush(QPixmap(":/resources/images/input.png"));
    view->setDragMode(QGraphicsView::ScrollHandDrag);


    //Button za start_battle
    QPushButton *bbattle =make_button("Start Battle");
    bbattle->setFixedWidth(200);
    bbattle->setFixedHeight(66);
    bbattle->move(980,600);
    QObject::connect(bbattle,SIGNAL(released()),this,SLOT(start()),Qt::QueuedConnection);
    QObject::connect(bbattle, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);

    //Button za back
    QPushButton *bback =make_button("Back");
    bback->setFixedWidth(200);
    bback->setFixedHeight(66);
    bback->move(100, 600);
    QObject::connect(bback, SIGNAL (released()), this, SLOT (main_menu()),Qt::QueuedConnection);
    QObject::connect(bback, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);

    //labeli za unos imena igraca
    QGraphicsTextItem *text1 =  new QGraphicsTextItem(QString("RED PLAYER NAME"));
    text1->setDefaultTextColor(Qt::red);
    text1->setFont(QFont("Times", 20, QFont::Bold));
    text1->setPos(250,250);
    scene->addItem(text1);

    QGraphicsTextItem *text2 =  new QGraphicsTextItem(QString("BLUE PLAYER NAME"));
    text2->setDefaultTextColor(Qt::blue);
    text2->setFont(QFont("Times", 20, QFont::Bold));
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
//    this->first_tank_name = line1->text();
//    this->second_tank_name = line2->text();

}
void World::change_name_of_first_tank() {
    if(line1->text().length() > 8){
        if(!m_showed_warning){

            m_showed_warning = true;

            QString info;
            info.reserve(30);
            info.append("The length of the name can't be longer than 8 letters");

            QFont font = QFont("Times", 12, QFont::Bold);

            warning_name_length = scene->addText(info, font);
            warning_name_length->setPos(597,310);
            warning_name_length->setDefaultTextColor(Qt::red);
        }
    }
    else {
        this->first_tank_name = line1->text();
    }
}

void World::change_name_of_second_tank() {
    if(line1->text().length() > 8){
        if(!m_showed_warning){

            m_showed_warning = true;

            QString info;
            info.reserve(30);
            info.append("The length of the name can't be longer than 8 letters");

            QFont font = QFont("Times", 12, QFont::Bold);

            warning_name_length = scene->addText(info, font);
            warning_name_length->setPos(597,310);
            warning_name_length->setDefaultTextColor(Qt::red);
        }
    }
    else {
        this->second_tank_name = line2->text();
    }
}

void World::quit() {
      //brisemo ceo world
      delete this;
      //ova funkcija brise scene i view tako da oni ne smeju da se nadju u ~world();
      QApplication::exit();
}
