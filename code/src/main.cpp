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
#include "code/include/Wall.hpp"
#include "code/include/Tank.hpp"
#include "code/include/World.hpp"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    World *world = new World();


    world->show();

    world->main_menu();

    //OBAVEZNO POGLEDAJTE OVO!
    //delete world;
    //posto smo alocirali world koji je nasa klasa zar ne bi trebalo da ga ovde deinic
    //ali kad uradim delete ne radi
    //pada mi napamet da se alocira world samo na steku
    //World world;
    //world.show();
    //world.main_menu();

    return a.exec();
}
