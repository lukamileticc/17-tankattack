#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QPixmap>
#include <QString>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include "code/include/Server.hpp"
#include "code/include/button.hpp"
#include "code/include/Wall.hpp"
#include "code/include/Tank.hpp"
#include "code/include/World.hpp"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    World *world = new World();

    Server *server = new Server();
    if (!server->listen(QHostAddress::Any, 1967)) {
        qDebug() << "Server hasn't started!";
    }
    else
        qDebug() << "Server has started";

    qDebug() << "mrk";

    world->show();

    world->main_menu();


    return a.exec();
}
