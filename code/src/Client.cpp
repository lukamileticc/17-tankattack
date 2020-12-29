#include "../include/Client.hpp"

#include <QTcpSocket>
#include <QDataStream>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QHostAddress>
bool tmp_Shoot = false;
//float x1;
//float y1;

qint16 movement;
Client::Client(QObject *parent)
    : QObject(parent)
    , m_clientSocket(new QTcpSocket(this))
    , m_loggedIn(false)
{

    m_pozicija_tenka_x = 200;
    m_pozicija_tenka_y = 400;
    connect(m_clientSocket, &QTcpSocket::connected, this, &Client::connected);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(m_clientSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::error);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, [this]()->void{m_loggedIn = false;});
   // connect(m_clientSocket, SIGNAL(), this, SLOT(sendMessage(QString)));
   // connect(ui->sendButton, &QPushButton::clicked, this, &ClientWindow::sendMessage);
}

void Client::login(const QString &userName)
{

    if (m_clientSocket->state() == QAbstractSocket::ConnectedState) { // if the client is connected
        QDataStream clientStream(m_clientSocket);
        clientStream.setVersion(QDataStream::Qt_5_7);
        QJsonObject message;
        message[QStringLiteral("type")] = QStringLiteral("login");
        message[QStringLiteral("username")] = userName;
        qDebug() << userName;
        clientStream << QJsonDocument(message).toJson(QJsonDocument::Compact);
    }
}

void Client::sendMessage(const QString &text)
{

    if (text.isEmpty())
        return;
    QDataStream clientStream(m_clientSocket);
    clientStream.setVersion(QDataStream::Qt_5_7);
    QJsonObject message;
    QString movement;
    qint16 movementId;
//    if(text != "Space")
//    {
//         message = {
//          {"x", m_pozicija_tenka_x},
//          {"y", m_pozicija_tenka_y},
//          {"id", 1}
//        };

//    }
    if(text == "up")
    {
        movement = "up";
        movementId = 1;
    }
    else if(text == "down")
    {
        movement = "down";
        movementId = 2;
    }
    else if(text == "left")
    {
        movement = "left";
        movementId = 3;
    }
    else if(text == "right")
    {
        movement = "right";
        movementId = 4;
    }
    else if(text == "left && up")
    {
        movement = "left && up";
        movementId = 5;
    }
    else if(text == "right && up")
    {
        movement = "right && up";
        movementId = 6;
    }
    else if(text == "left && down")
    {
        movement = "left && down";
        movementId = 7;
    }
    else if(text == "righ && down")
    {
        movement = "right && down";
        movementId = 8;
    }
    else if(text == "Space")
    {
        movement = "Space";
        movementId = 9;
    }

    message = {
        {movement, movementId}
    };
    qDebug() << message;

    clientStream << QJsonDocument(message).toJson();

}

void Client::disconnectFromHost()
{
    m_clientSocket->disconnectFromHost();
}

void Client::jsonReceived(const QJsonObject &docObj)
{
    if(*docObj.find("Space") != *docObj.end())
    {
        tmp_Shoot = true;
    }
     QJsonValue movements = *docObj.begin();
     movement = movements.toInt();

}

void Client::connectToServer(const QHostAddress &address, quint16 port)
{
    m_clientSocket->connectToHost(QHostAddress::LocalHost, 1967);
}
void Client::onReadyRead()
{
    QByteArray jsonData;
    QDataStream socketStream(m_clientSocket);
    socketStream.setVersion(QDataStream::Qt_5_7);
    for (;;) {
        socketStream.startTransaction();
        socketStream >> jsonData;
        if (socketStream.commitTransaction()) {

            QJsonParseError parseError;
            const QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);
            if (parseError.error == QJsonParseError::NoError) {
                if (jsonDoc.isObject()){
                    jsonReceived(jsonDoc.object());
                }
            }
        } else {

            break;
        }
    }
}

//void Client::setPozicija_TenkaX(float pozicija_tenka_x)
//{
//    m_pozicija_tenka_x = pozicija_tenka_x;
//}
//void Client::setPozicija_TenkaY(float pozicija_tenka_y)
//{
//    m_pozicija_tenka_y = pozicija_tenka_y;
//}

//float Client::getPozicija_TenkaX()
//{
//    return m_pozicija_tenka_x;
//}

//float Client::getPozicija_TenkaY()
//{
//    return m_pozicija_tenka_y;
//}

//float Client::getX_Primljeno()
//{
//    return x1;
//}
//float Client::getY_Primljeno()
//{
//    return y1;
//}

bool Client::isOrderedToShoot()
{
    return tmp_Shoot;
}

void Client::setCantShoot()
{
    tmp_Shoot = false;
}


qint16 Client::getMovement()
{
    return movement;
}
void Client::nullifyMovement()
{
    movement = 0;
}

