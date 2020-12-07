#ifndef WORLD_HPP
#define WORLD_HPP

#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsView>
#include <QObject>

class World : public QObject
{
    Q_OBJECT

public:
    World(QObject *parent = nullptr);

    ~World();

    void show();

    QGraphicsScene *scene = nullptr;
    QGraphicsView  *view  = nullptr;

public slots:
    void start();
    void quit();
    void main_menu();
    void show_battles();

private:
    int m_in_game=0;
    int m_started=0;
};

#endif // WORLD_HPP
