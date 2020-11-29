#ifndef WORLD_HPP
#define WORLD_HPP

#include "../include/rocket_1.hpp"
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsView>
#include <QObject>
#include <QPushButton>
#include <iostream>
#include <QBrush>
#include <QApplication>
#include <QDebug> //biblioteka za debagovanje koristi se ovako: qDebug() << "we are here";
#include "code/include/Tank.hpp"
class World : public QObject
{
    Q_OBJECT

public:
    World(QObject *parent = nullptr);

    virtual ~World();

    void show();

    void main_menu();

    QGraphicsScene* getScene() const{
        return m_scene;
    }


    QGraphicsScene *m_scene = nullptr;
    QGraphicsView  *m_view  = nullptr;


public slots:
    void start();
    void quit();

private:
    //World mora da ima tenkove
    Tank* t1 = nullptr;
    Tank* t2 = nullptr;

};

#endif // WORLD_HPP
