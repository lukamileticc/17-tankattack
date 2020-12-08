#ifndef WORLD_HPP
#define WORLD_HPP

#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsView>
#include <QObject>
#include <string>
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
    Tank *t1 = nullptr;
    Tank *t2 = nullptr;

    void end_of_round(std::string message);

public slots:
    void start();
    void quit();
    void main_menu();
    void show_battles();
    void rounds();

private:
    int m_in_game=0;
    int m_started=0;
    int m_skor_t1 = 0;
    int m_skor_t2 = 0;
};

#endif // WORLD_HPP
