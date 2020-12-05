#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = 0);
    void Test();

signals:


public slots:
    void connected();
    void disconnected();
    //bytes je broj procitanih bajtova
    void bytesWritten(qint64 bytes);
    //kad mozemo da citamo
    void readyRead();



private:
    //Deklarise soket
    QTcpSocket *socket;

};

#endif // CLIENT_HPP
