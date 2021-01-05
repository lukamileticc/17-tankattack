#ifndef WORLD_HPP
#define WORLD_HPP

#include "code/include/Tank.hpp"
#include "code/include/Map.hpp"
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsView>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QLineEdit>
#include <QMediaPlayer>
#include <QPushButton>


class World : public QObject {
Q_OBJECT

public:
    World(QObject *parent = nullptr);

    ~World();

    void show();

    QGraphicsScene *scene = nullptr;
    QGraphicsView  *view  = nullptr;
    QGraphicsTextItem *info_t1 = nullptr;
    QGraphicsTextItem *info_t2 = nullptr;
    QGraphicsTextItem *game_score = nullptr;
    QGraphicsTextItem *info_pause = nullptr;
    QGraphicsTextItem *warning_name_length = nullptr;
    Tank *t1 = nullptr;
    Tank *t2 = nullptr;
    QLineEdit *line1 = nullptr;
    QLineEdit *line2 = nullptr;
    QMediaPlayer *music = nullptr;
    QMediaPlayer *button_sound = nullptr;
    Input *input = nullptr;
    Map* m = nullptr; //m je map


    static bool world_pause;
    static bool isHosting;
    QPushButton* make_button(QString name);
    void end_of_round(QString message);
    void show_tank_info();
    void pause();

    //funkcije koje vrse ispis i upis borbi
    QVector<QString>* read_previous_battles(const char *file);
    void write_the_last_battle(const char *file);

    void end_game_screen(QString message);
    void volumen_first();

public slots:
    void start();
    void quit();
    void main_menu();
    void show_battles();
    void input_players_names();
    void rounds();
    void change_name_of_first_tank();
    void change_name_of_second_tank();
    void load_map();
    void button_clicked();
    void set_volume();
    void multiplayer_menu();
    void start_server();
    void find_game();
    void generate_superpower();

private:
    int m_in_game=0;
    int m_started=0;
    int m_score_t1 = 0;
    int m_score_t2 = 0;
    bool m_ended_round = false;
    bool m_showed_info = false;
    bool m_showed_pause = false;
    bool m_showed_warning = false;
    QString first_tank_name = "Player 1";
    QString second_tank_name = "Player 2";
    int m_last_map = -1; // number of last map
    int m_left_round_time = 0; /* timer povecava ovaj brojac kako bi proslo odredjeno vreme nakon
                               unistavanja jednog od tenkova (mozda zalutala raketa unisti drugi tenk)*/

    //polje koje sluzi da menja sliku dugmeta za pojacavanje tona muzike
    QPushButton* bvolumen = nullptr;

    bool isMultiplayer = false;

    //flag koji oznacava da li smo vec bili u main_menu
    bool main_menu_flag = false;
};

#endif // WORLD_HPP
