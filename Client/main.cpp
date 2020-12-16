#include "ClientWindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ClientWindow cw;
    cw.show();
    return a.exec();
}
