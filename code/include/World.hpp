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

    ~World() = default;

    void show();

    void main_menu();

public slots:
    void start();
    void quit();

protected:
    QGraphicsScene *scene = nullptr;
    QGraphicsView  *view  = nullptr;

};

#endif // WORLD_HPP
