#include "../include/Client.hpp"

#include <QTcpSocket>
#include <QDataStream>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QHostAddress>
bool tmp_Shoot = false;
bool start_the_game = false;
float x1;
float y1;
float angle1;
qint16 movement;
Client::Client(QObject *parent)
    : QObject(parent)
    , m_clientSocket(new QTcpSocket(this))
    , m_loggedIn(false)
{


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
    if(text != "Space")
    {
         message = {
          {"x", m_pozicija_tenka_x},
          {"y", m_pozicija_tenka_y},
          {"angle", m_angle}
        };

    }

    else if(text == "Space")
    {
        movement = "Space";
        movementId = 9;

        message = {
            {movement, movementId}
        };
    }


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

     else {
        QJsonValue xs = *docObj.find("x");
        QJsonValue ys = *docObj.find("y");
        QJsonValue angles = *docObj.find("angle");

        Client::x_Primljeno = xs.toDouble();
        Client::y_Primljeno = ys.toDouble();
        Client::angle_Primljeno = angles.toDouble();

        x1 = x_Primljeno;
        y1 = y_Primljeno;
        angle1 = angle_Primljeno;

        qDebug() << angle1 << "ANGLE";
    }
}

void Client::connectToServer(const QHostAddress &address, quint16 port)
{
    m_clientSocket->connectToHost(QHostAddress::LocalHost, 1967);

}
void Client::onReadyRead()
{
    qDebug() << "NISTA";
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

void Client::nullifyX()
{
    x1 = 0;
}

void Client::nullifyY()
{
    y1 = 0;
}

void Client::nullifyAngle()
{
    angle1 = 0;
}
void Client::setAngle_Tenka(float angle)
{
   m_angle = angle;
}

void Client::setPozicija_TenkaX(float pozicija_tenka_x)
{
    m_pozicija_tenka_x = pozicija_tenka_x;
}

void Client::setPozicija_TenkaY(float pozicija_tenka_y)
{
    m_pozicija_tenka_y = pozicija_tenka_y;
}

float Client::getPozicija_TenkaX()
{
    return m_pozicija_tenka_x;
}

float Client::getPozicija_TenkaY()
{
    return m_pozicija_tenka_y;
}

float Client::getX_Primljeno()
{
    return x1;
}
float Client::getY_Primljeno()
{
    return y1;
}

float Client::getAngle_Primljeno()
{
    return angle1;
}


