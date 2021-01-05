#include "../include/Client.hpp"

#include <QTcpSocket>
#include <QDataStream>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QHostAddress>
#include <QHostInfo>

//da li drugi igrac puca
bool tmp_Shoot = false;
//Koordinate i ugao koje primamo od drugog igraca
float x_received;
float y_received;
float angle_received;
Client::Client(QObject *parent)
    : QObject(parent)
    , m_clientSocket(new QTcpSocket(this))
    , m_loggedIn(false)
{
    x_received = 0;
    y_received = 0;
    angle_received = 0;

    connect(m_clientSocket, &QTcpSocket::connected, this, &Client::connected);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(m_clientSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &Client::error);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, [this]()->void{m_loggedIn = false;});
}

void Client::login(const QString &userName)
{

    if (m_clientSocket->state() == QAbstractSocket::ConnectedState) { // if the client is connected
        QDataStream clientStream(m_clientSocket);
        clientStream.setVersion(QDataStream::Qt_5_7);
        QJsonObject message;
        message[QStringLiteral("type")] = QStringLiteral("login");
        message[QStringLiteral("username")] = userName;
        clientStream << QJsonDocument(message).toJson(QJsonDocument::Compact);
    }
}

Client::~Client()
{
    disconnectFromHost();
}

void Client::sendMessage(const QString &text)
{

    if (text.isEmpty())
        return;
    QDataStream clientStream(m_clientSocket);
    clientStream.setVersion(QDataStream::Qt_5_7);
    QJsonObject message;

    //posalji koordinate tenka
    if(text != "Space")
    {
         message = {
          {"x", m_TankX},
          {"y", m_TankY},
          {"angle", m_angle}
        };
    }
    //posalji da treba da puca
    else if(text == "Space")
    {
        message = {
            {text, 9}
        };
    }
    clientStream << QJsonDocument(message).toJson();
}

void Client::disconnectFromHost()
{
    m_clientSocket->disconnectFromHost();
}

void Client::jsonReceived(const QJsonObject &docObj)
{
    //primio je da treba da puca
    if(*docObj.find("Space") != *docObj.end())
    {
        tmp_Shoot = true;
    }
     //primio je da treba da se pomera
     else {
        QJsonValue xs = *docObj.find("x");
        QJsonValue ys = *docObj.find("y");
        QJsonValue angles = *docObj.find("angle");

        x_received = xs.toDouble();
        y_received = ys.toDouble();
        angle_received = angles.toDouble();
    }
}

void Client::connectToServer(const QString &ipAdress, quint16 port)
{
    m_clientSocket->connectToHost(ipAdress, port);
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

bool Client::isOrderedToShoot()
{
    return tmp_Shoot;
}
void Client::setCantShoot()
{
    tmp_Shoot = false;
}

void Client::setTanksAngle(float angle)
{
   m_angle = angle;
}
void Client::setTanksX(float TankX)
{
    m_TankX = TankX;
}
void Client::setTanksY(float TankY)
{
    m_TankY = TankY;
}

float Client::getTankX()
{
    return m_TankX;
}
float Client::getTankY()
{
    return m_TankY;
}


float Client::getReceivedX()
{
    return x_received;
}
float Client::getReceivedY()
{
    return y_received;
}
float Client::getReceivedAngle()
{
    return angle_received;
}


