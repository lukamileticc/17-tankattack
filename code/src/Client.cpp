#include "../include/Client.hpp"

Client::Client(QObject *parent)
    :QObject(parent)
{

}


void Client::Test()
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));

    qDebug() << "Konektuje se";

    socket->connectToHost("google.com", 80);

    if(!socket->waitForConnected(1000))
    {
        qDebug() << "Waiting" << socket->errorString();
    }


}
void Client::connected()
{
     qDebug() << "connected";

     socket->write("HEAD / HTTP/1.0\r\n\r\n\r\n");
}
void Client::disconnected()
{
     qDebug() << "disconnected";
}
//bytes je broj procitanih bajtova
void Client::bytesWritten(qint64 bytes)
{
     qDebug() << "bytesWritten" << bytes;
}
//kad mozemo da citamo
void Client::readyRead()
{
     qDebug() << "readyRead";
      qDebug() << socket->readAll();
}
