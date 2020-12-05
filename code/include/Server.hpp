#ifndef SERVER_HPP
#define SERVER_HPP

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject * parent=0);


signals:



public slots:
    void newConnection();

private:
    QTcpServer *server;


 };

#endif // SERVER_HPP
