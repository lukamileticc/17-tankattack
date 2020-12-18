#ifndef WORLD_HPP
#define WORLD_HPP

#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsView>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QLineEdit>
#include "code/include/Tank.hpp"

class World : public QObject
{
    Q_OBJECT

public:
    World(QObject *parent = nullptr);

    ~World();

    void show();

    QGraphicsScene *scene = nullptr;
    QGraphicsView  *view  = nullptr;
    QGraphicsTextItem *info_t1 = nullptr;
    QGraphicsTextItem *info_t2 = nullptr;
    Tank *t1 = nullptr;
    Tank *t2 = nullptr;
    QLineEdit* line1 = nullptr;
    QLineEdit* line2 = nullptr;

    void end_of_round(QString message);
    void show_tank_info();

    //funkcije koje vrse ispis i upis borbi
    QVector<QString>* read_previous_battles(const char *file);
    void write_the_last_battle(const char *file);

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

private:
    int m_in_game=0;
    int m_started=0;
    int m_score_t1 = 0;
    int m_score_t2 = 0;
    bool m_ended_round = false;
    bool m_showed_info = false;
    QString ime_prvog_tenka;
    QString ime_drugog_tenka;
    int m_last_map = -1; // number of last map
    int m_left_round_time = 0; // timer povecava ovaj brojac kako bi proslo odredjeno vreme nakon
                               //unistavanja jednog od tenkova (mozda zalutala raketa unisti drugi tenk)
};

#endif // WORLD_HPP
