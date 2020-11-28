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

    void main_menu();

    QGraphicsScene* getScene() const{
        return m_scene;
    }

public slots:
    void start();
    void quit();

protected:
    QGraphicsScene *m_scene = nullptr;
    QGraphicsView  *m_view  = nullptr;

};

#endif // WORLD_HPP
