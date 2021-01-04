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
#define NUM_OF_MAPS 3

bool World::world_pause;
bool World::isHosting = false;

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

World::~World() {
    //UNISTEN TENK SE SAM UNISTAVA , s tim sto na kraju svake runde treba unisti oba tenka
    //jer se u sledece pozivu next_round ona oba opet inicijalizuju
    //TENKOVI SE PRAVILNO OSLOBADJAJU!
    //I input se pravilno oslobadja! i map(m) se pravilno oslobadja

    //muzika se pravilno oslobadja
    delete music;
    //i button sound se ispravno deiicijalizuje
    delete  button_sound;
    //pravilnoo
    delete bvolumen;

 // info_t2 i info_t1 i game_score i info_pause se pravilno oslobadjaja


//OVO DOLE NE ZNAM DA OSLOBODIM!

//    delete line2;
//    delete line1;
//    delete warning_name_length;

}

void World::show(){
    view->setWindowTitle("Tank Attack");
    view->setFixedSize(1150, 768);
    view->show();
}

void World::main_menu() {

    if(main_menu_flag == true)
    {
        //ako je true vec smo ulazili u main samim tim inicijalizovali bvolumen
        delete bvolumen;
        main_menu_flag = true;
    }
    if(m_showed_info == true)
    {
        delete info_t1;
        delete info_t2;
        delete game_score;
        m_showed_info = false;
    }

    //ako se zavrsila pesma ,onda se opet pusta
    if(music->state() == QMediaPlayer::StoppedState
    || music->state() == QMediaPlayer::PausedState)
    {
        qDebug() << music->state();
        music->play();
    }
    m_showed_warning = false;
    m_in_game = 0;
    m_score_t1 = 0;
    m_score_t2 = 0;

    m_last_map = 0;


    scene->clear();
    view->setBackgroundBrush(QPixmap(":/resources/images/main_menu.png"));


    //button za pojacavanje i smanjivanje volumena
    bvolumen = make_button("VOLUME");
    bvolumen->move(scene->width() - 100, scene->height() - 100);
//    QPixmap pixmap(":/resources/images/volume_3.png");
//    bvolumen->setIcon(QIcon(pixmap));
    volumen_first();
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
    bstart->move(scene->width() / 2 - bstart->rect().width() / 2, 200);
    QObject::connect(bstart, SIGNAL (released()), this, SLOT (input_players_names()), Qt::QueuedConnection);
    QObject::connect(bstart, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);

   
//    if (m_started == 1){
//        QPushButton *bcontinue = make_button("CONTINUE GAME");

//        bcontinue->setFixedWidth(300);
//        bcontinue->setFixedHeight(100);
//        bcontinue->move(scene->width()/2 - bstart->rect().width()/2, 625);
//        QObject::connect(bcontinue, SIGNAL (released()), this, SLOT (start()), Qt::QueuedConnection);
//        QObject::connect(bcontinue, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);
//    }

    //Button za mrezni pultiplayer
    QPushButton *bmultiplayer = make_button("MULTIPLAYER");
    bmultiplayer->setFixedWidth(300);
    bmultiplayer->setFixedHeight(100);
    bmultiplayer->move(scene->width() / 2 - bmultiplayer->rect().width() / 2, 325);
    QObject::connect(bmultiplayer, SIGNAL(released()), this, SLOT (multiplayer_menu()), Qt::QueuedConnection);
    QObject::connect(bmultiplayer, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);

    //Button za prikaz prethodnih score-ova
    QPushButton *bbattle = make_button("BATTLES");
    bbattle->setFixedWidth(300);
    bbattle->setFixedHeight(100);
    bbattle->move(scene->width() / 2 - bbattle->rect().width() / 2, 450);
    QObject::connect(bbattle, SIGNAL (released()), this, SLOT (show_battles()), Qt::QueuedConnection);
    QObject::connect(bbattle, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);


    //Button za izlaz
    QPushButton *bquit = make_button("QUIT GAME");
    bquit->setFixedWidth(300);
    bquit->setFixedHeight(100);
    bquit->move(scene->width() / 2 - bquit->rect().width() / 2, 575);
    QObject::connect(bquit, SIGNAL (released()), this, SLOT (quit()));
    QObject::connect(bquit, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);
}

void World::start() {
    music->pause();
    m_ended_round = false;
    if(m_showed_info == true)
    {
        delete info_t1;
        delete info_t2;
        delete game_score;
        m_showed_info = false;
    }
    m_showed_info = false;
    m_showed_warning = false;
    World::world_pause = false;
    Rocket::rakete_tenka_0 = 0;
    Rocket::rakete_tenka_1 = 0;
    m_left_round_time = 0;
    m_started = 1;
    m_in_game = 1;

    scene->clear();
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setFixedSize(1150, 768);


    input = new Input();
    QColor host = Qt::blue;
    QColor client = Qt::red;

    if(isHosting) {
        host = Qt::red;
        client = Qt::blue;
    }

    this->t1 = new Tank(0,Qt::red, 80, 500, input, host, client, isMultiplayer);
    this->t2 = new Tank(1,Qt::blue, 1045, 500, input, host, client, isMultiplayer);

    t1->set_name(this->first_tank_name);
    t2->set_name(this->second_tank_name);

//    SuperPower *sp= new SuperPower("superpower",QRandomGenerator::global()->bounded(1240),QRandomGenerator::global()->bounded(600),30);
//    SuperPower *sp1= new SuperPower("health",QRandomGenerator::global()->bounded(1240),QRandomGenerator::global()->bounded(600),30);
//    SuperPower *sp2= new SuperPower("speed",QRandomGenerator::global()->bounded(1240),QRandomGenerator::global()->bounded(600),30);

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

//    qDebug() << "helloooo";
//    qDebug() << "Ime prvog tenka: " << t1->get_name();
//    qDebug() << "Ime drugog tenka: " << t2->get_name();
}

void World::multiplayer_menu() {
    isMultiplayer = true;
    scene->clear();

    view->setBackgroundBrush(QPixmap(":/resources/images/mp_menu.png"));

    QPushButton *bhost = make_button("Host game");
    bhost->setFixedWidth(300);
    bhost->setFixedHeight(100);
    bhost->move(scene->width() / 2 - bhost->rect().width() / 2, scene->height() / 2 - 120);
    QObject::connect(bhost, SIGNAL (released()), this, SLOT (start_server()),Qt::QueuedConnection);
    QObject::connect(bhost, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);

    QPushButton *bfind = make_button("Find game");
    bfind->setFixedWidth(300);
    bfind->setFixedHeight(100);
    bfind->move(scene->width() / 2 - bfind->rect().width() / 2, scene->height() / 2 + 5);
    QObject::connect(bfind, SIGNAL (released()), this, SLOT (find_game()),Qt::QueuedConnection);
    QObject::connect(bfind, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);

    QPushButton *bback = make_button("Back");
    bback->setFixedWidth(200);
    bback->setFixedHeight(66);
    bback->move(25 , scene->height() - bback->rect().height() - 25);
    QObject::connect(bback, SIGNAL (released()), this, SLOT (main_menu()),Qt::QueuedConnection);
    QObject::connect(bback, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);
}

void World::show_battles() {
    scene->clear();

    view->setBackgroundBrush(QPixmap(":/resources/images/battles_log.png"));
    view->setDragMode(QGraphicsView::ScrollHandDrag);

    QPushButton *bback = make_button("Back");
    bback->setFixedWidth(200);
    bback->setFixedHeight(66);
    bback->move(scene->width() - bback->rect().width() - 25, scene->height() - bback->rect().height() - 25);
    QObject::connect(bback, SIGNAL (released()), this, SLOT (main_menu()), Qt::QueuedConnection);
    QObject::connect(bback, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);

    //citamo prethodne borbe iz funkcije
    QVector<QString> *previous_battles = nullptr;

    try {
        previous_battles = read_previous_battles("../17-tankattack/code/res/istorija_borbi.txt");
        if(previous_battles != nullptr)
            qDebug() << "Tu sam";
    }
    catch(const QString &e) {
       qDebug() << e;
       delete previous_battles;
       exit(EXIT_FAILURE);
    }

     QString title = "LAST 10 BATTLES:\n";
     QString pair_list;

     pair_list.append("\n\n");

     int i = 1;
     QString tab = "";

     for(const auto& battle : *previous_battles)
        pair_list.append("\n").append(tab)
                .append(QString::number(i++)).append(".  ")
                .append(battle);


    QFont font("Helvetica", 40, QFont::Bold);
    QFont font_lista("Helvetica", 18);
    QGraphicsTextItem *text = scene->addText(title, font);
    int title_x_pos = scene->width() / 2 - text->boundingRect().width() / 2;
    text->setPos(title_x_pos, 0);
    text->setDefaultTextColor(QColor("white"));

    QGraphicsTextItem *text_list = scene->addText(pair_list, font_lista);
    int list_x_pos = scene->width() / 2 - text_list->boundingRect().width() / 2;
    text_list->setPos(list_x_pos, 0);
    text_list->setDefaultTextColor(QColor("white"));

    //brisemo pokazivac na vektor
    delete previous_battles;
}

void World::start_server() {
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

void World::find_game() {
    start();
}

int rand_int(unsigned num_of_maps) {
    srand(time(NULL));
    return (rand() % num_of_maps) + 1;
}

void World::load_map() {
    char map[] = ":/resources/files/mapa?.txt";
    char map_texture[] = ":/resources/images/map_?_background.png";

    int random_integer;
    if(m_last_map != 3)
    {
        random_integer = ++m_last_map;
    }
    else
        random_integer = 1;

    m_last_map = random_integer;

    char number_of_map[2];
    std::sprintf(number_of_map, "%d", random_integer);
    map[22] = number_of_map[0];
    map_texture[23] = number_of_map[0];

    view->setBackgroundBrush(QPixmap(map_texture));

    m = new Map(map);
    std::vector<Wall *> walls = m->getWalls();

    for(auto w: walls)
        scene->addItem(w);

    //delete m; ovo ne moze ovde!
}

void World::rounds() {
    if(m_ended_round)
        return;

    if(t1->is_pause() || t2->is_pause()){
        world_pause = true;
        pause();
        return;
    }
    else{
        if (world_pause){
            world_pause = false;
            m_showed_pause = false;
            scene->removeItem(info_pause);
            delete info_pause;
        }
        //Da se ne bi ispisivalo cant remove from the scene
        //ovde se treba proveriti da li je info_pause na sceni
        //ako jeste onda ga brisi! a ako nije da uopste ne ulazi ovde!

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

void World::end_of_round(QString message) {
    //show_tank_info();
    //std::cout << message << std::endl;
    m_ended_round = true;

    show_tank_info();

    if (m_score_t1 == 3 || m_score_t2 == 3) {
        end_game_screen(message);
        write_the_last_battle("../17-tankattack/code/res/istorija_borbi.txt");
    }
    else {
        QFont font("Helvetica", 50, QFont::Bold);

        QGraphicsTextItem *text = scene->addText(message + " won round!", font);
        int x_position = scene->width() / 2 - text->boundingRect().width() / 2;
        text->setPos(x_position, 200);
        text->setDefaultTextColor(QColor("white"));
        //Button za next battle
        QPushButton *bnext = make_button("Next Round");
        bnext->setFixedWidth(200);
        bnext->setFixedHeight(66);
        bnext->move(scene->width() / 2 - bnext->rect().width() / 2, scene->height() / 2 - 45);
        QObject::connect(bnext, SIGNAL (released()), this, SLOT (start()), Qt::QueuedConnection);
        QObject::connect(bnext, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);


        //Button za main menu
        QPushButton *bm_menu = make_button("Main Menu");
        bm_menu->setFixedWidth(200);
        bm_menu->setFixedHeight(66);
        bm_menu->move(scene->width() / 2 - bm_menu->rect().width() / 2, scene->height() / 2 + 30);
        QObject::connect(bm_menu, SIGNAL (released()), this, SLOT (main_menu()), Qt::QueuedConnection);
        QObject::connect(bm_menu, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);
    }


    //moramo osloboditi oba tenka jer ce u sledecoj rundi da budu opet inicijalizovani
    //tek ovde Brisemo tenkove
    delete t1;
    delete t2;
    //Ovde se brise i input jer ce se kao i tenkovi opet inicijalizovati u sledecoj rundi
    delete input;
    //ovde mora i mapa da se brise mora! jer ce se ponovo inicijalizovati u startu!
    delete m;

    return;
}

void World::pause() {
    if(m_showed_pause) {
        scene->removeItem(info_pause);
        //da ne bi curela memorija
        delete info_pause;
    }

    m_showed_pause = true;
    QFont font("Helvetica", 50, QFont::Bold);
    QString pause_string;
    pause_string.reserve(50);
    pause_string.append("Pause! Press P to continue");

    //ovde se inicijalizuje info_pause ,znaci ako se vise puta pozove pause() ,ovde ce da curi
    //zato je potrebno gore delete info_pause
    info_pause = scene->addText(pause_string, font);
    int x_position = scene->width() / 2 - info_pause->boundingRect().width() / 2;
    info_pause->setPos(x_position, 300);
    info_pause->setDefaultTextColor(QColor("white"));
}

void World::end_game_screen(QString message) {

    QFont font("Helvetica", 50, QFont::Bold);

    QGraphicsTextItem *text = scene->addText(message + " won match!", font);
    int x_position = scene->width() / 2 - text->boundingRect().width() / 2;
    text->setPos(x_position, 200);
    text->setDefaultTextColor(QColor("white"));

    //Button za main menu
    QPushButton *bm_menu = make_button("Main Menu");
    bm_menu->setFixedWidth(200);
    bm_menu->setFixedHeight(66);
    bm_menu->move(scene->width() / 2 - bm_menu->rect().width() / 2, scene->height() / 2 - 50);
    QObject::connect(bm_menu, SIGNAL (released()), this, SLOT (main_menu()), Qt::QueuedConnection);
    QObject::connect(bm_menu, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);

    return;
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
    int text_x_position = scene->width() / 2 - game_score->boundingRect().width() / 2;
    game_score->setPos(text_x_position, 702);
    game_score->setDefaultTextColor("white");
}

QVector<QString>* World::read_previous_battles(const char *file) {
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

void World::write_the_last_battle(const char *file) {
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
    last_battle.append(first_tank_name).append(" ").append(" [")
               .append(QString::number(m_score_t1)).append(" ")
               .append("- ")
               .append(QString::number(m_score_t2)).append("] ")
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

void World::input_players_names() {
    scene->clear();
    view->setBackgroundBrush(QPixmap(":/resources/images/input_menu.png"));
    view->setDragMode(QGraphicsView::ScrollHandDrag);

    //Button za start_battle
    QPushButton *bbattle = make_button("Start Battle");
    bbattle->setFixedWidth(200);
    bbattle->setFixedHeight(66);
    bbattle->move(scene->width() - bbattle->rect().width() - 25, scene->height() - bbattle->rect().height() - 25);
    QObject::connect(bbattle,SIGNAL(released()),this,SLOT(start()),Qt::QueuedConnection);
    QObject::connect(bbattle, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);

    //Button za back
    QPushButton *bback = make_button("Back");
    bback->setFixedWidth(200);
    bback->setFixedHeight(66);
    bback->move(25, scene->height() - bback->rect().height() - 25);
    QObject::connect(bback, SIGNAL (released()), this, SLOT (main_menu()),Qt::QueuedConnection);
    QObject::connect(bback, SIGNAL(pressed()), this, SLOT (button_clicked()), Qt::QueuedConnection);

    //labeli za unos imena igraca
    QGraphicsTextItem *text1 =  new QGraphicsTextItem(QString("RED PLAYER NAME"));
    text1->setDefaultTextColor(Qt::black);
    text1->setFont(QFont("Helvetica", 20, QFont::Bold));
    text1->setPos(250, 250);
    scene->addItem(text1);

    QGraphicsTextItem *text2 =  new QGraphicsTextItem(QString("BLUE PLAYER NAME"));
    text2->setDefaultTextColor(Qt::black);
    text2->setFont(QFont("Helvetica", 20, QFont::Bold));
    text2->setPos(250, 350);
    scene->addItem(text2);


    QFont font = QFont("Helvetica", 12, QFont::Thin);

    //line edit za unos imena igraca
    line1 = new QLineEdit("Enter red tank name");
    line1->setGeometry(600, 250, 310, 40);
    line1->setClearButtonEnabled(true);
    line1->setFont(font);
    scene->addWidget(line1);


    line2= new QLineEdit("Enter blue tank name");
    line2->setGeometry(600, 350, 310, 40);
    line2->setClearButtonEnabled(true);
    line2->setFont(font);
    scene->addWidget(line2);


    QObject::connect(line1, SIGNAL(textChanged(QString)), this, SLOT(change_name_of_first_tank()), Qt::QueuedConnection);
    QObject::connect(line2, SIGNAL(textChanged(QString)), this, SLOT(change_name_of_second_tank()), Qt::QueuedConnection);
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

QPushButton* World::make_button(QString name) {

    QString style_for_buttons = "QPushButton"
                              "{"
                              "background-color: #CCCCCC;"
                              "border-style: outset;"
                              "border-width: 2.5px;"
                              "border-radius: 10px;"
                              "border-color: beige;"
                              "font: bold 16px;"
                              "padding: 6px;"
                              "}"
                              "QPushButton::hover"
                              "{"
                              "background-color: #EAEAEA"
                              "}";


    QPushButton *button = new QPushButton(name);
    button->setAttribute(Qt::WA_TranslucentBackground);
    button->setStyleSheet(style_for_buttons);
    scene->addWidget(button);

    return button;
}

void World::button_clicked() {
    button_sound->play();
}
void World::volumen_first()
{
    if(music->volume() == 0) {
        QPixmap pixmap(":/resources/images/volume_off.png");
        bvolumen->setIcon(QIcon(pixmap));
    }
    else if(music->volume() == 10) {
        QPixmap pixmap(":/resources/images/volume_1.png");
        bvolumen->setIcon(QIcon(pixmap));
   }
    else if(music->volume() == 20) {
        QPixmap pixmap(":/resources/images/volume_2.png");
        bvolumen->setIcon(QIcon(pixmap));
    }
    else if(music->volume() == 30) {
        QPixmap pixmap(":/resources/images/volume_3.png");
        bvolumen->setIcon(QIcon(pixmap));
    }

}
void World::set_volume() {
    if(music->volume() == 0) {
        QPixmap pixmap(":/resources/images/volume_3.png");
        bvolumen->setIcon(QIcon(pixmap));
        music->setVolume(30);
    }
    else if(music->volume() == 10) {
        QPixmap pixmap(":/resources/images/volume_off.png");
        bvolumen->setIcon(QIcon(pixmap));
        music->setVolume(0);
    }
    else if(music->volume() == 20) {
        QPixmap pixmap(":/resources/images/volume_1.png");
        bvolumen->setIcon(QIcon(pixmap));
        music->setVolume(10);
    }
    else if(music->volume() == 30) {
        QPixmap pixmap(":/resources/images/volume_2.png");
        bvolumen->setIcon(QIcon(pixmap));
        music->setVolume(20);
    }
}

void World::quit() {
      //brisemo ceo world
      delete this;
      //ova funkcija brise scene i view tako da oni ne smeju da se nadju u ~world();
      QApplication::exit();
}
