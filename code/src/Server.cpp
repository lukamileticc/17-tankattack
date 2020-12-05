#include "../include/Server.hpp"
#include <iostream>
#include <QDebug>
Server::Server(QObject *parent) :
    QObject(parent)
{
    server=new QTcpServer(this);
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));
    if(!server->listen(QHostAddress::Any,1234)){
        std::cout<<"Nije napravljena nova konekcija";

    }
    else {
        std::cout<< "Napravljena konekcija";
    }
}

void Server::newConnection()
{
    QTcpSocket *socket= server->nextPendingConnection();

    socket->write("Hello client\n");
    socket->flush();


    socket->waitForBytesWritten(3000);


    socket->close();
}
