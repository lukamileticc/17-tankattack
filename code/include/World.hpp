#ifndef WORLD_HPP
#define WORLD_HPP
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsView>
#include <QObject>
#include <QPushButton>
#include <iostream>
#include <QBrush>
#include <QApplication>
#include <QDebug> //biblioteka za debagovanje koristi se ovako: qDebug() << "we are here";

class World : public QObject
{
    Q_OBJECT

public:
    World(QObject *parent = nullptr);

    virtual ~World();

    void show();


public slots:
    void start();
    void quit();
    void main_menu();
    void show_battles();
protected:
    QGraphicsScene *scene = nullptr;
    QGraphicsView  *view  = nullptr;
private:
    int m_in_game=0;
    int m_started=0;
};

#endif // WORLD_HPP
