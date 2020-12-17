#include "../include/Client.hpp"

#include <QTcpSocket>
#include <QDataStream>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QHostAddress>

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
   // connect(m_clientSocket, SIGNAL(), this, SLOT(sendMessage(QString)));
   // connect(ui->sendButton, &QPushButton::clicked, this, &ClientWindow::sendMessage);
}

void Client::login(const QString &userName)
{
    m_pozicija_tenka_x = 0;
    m_pozicija_tenka_y = 0;
    if (m_clientSocket->state() == QAbstractSocket::ConnectedState) { // if the client is connected
        QDataStream clientStream(m_clientSocket);
        clientStream.setVersion(QDataStream::Qt_5_7);
        QJsonObject message;
        message[QStringLiteral("type")] = QStringLiteral("login");
        message[QStringLiteral("username")] = userName;
        clientStream << QJsonDocument(message).toJson(QJsonDocument::Compact);
    }
}

void Client::sendMessage(const QString &text)
{

    if (text.isEmpty())
        return;
    QDataStream clientStream(m_clientSocket);
    clientStream.setVersion(QDataStream::Qt_5_7);
    QJsonObject message = {
      {"x", m_pozicija_tenka_x},
      {"y", m_pozicija_tenka_y}
    };
    qDebug() << getPozicija_TenkaX();

    //message[QStringLiteral("type")] = QStringLiteral("message");
    //message[QStringLiteral("text")] = text;
    clientStream << QJsonDocument(message).toJson();

}

void Client::disconnectFromHost()
{
    m_clientSocket->disconnectFromHost();
}

void Client::jsonReceived(const QJsonObject &docObj)
{
    const QJsonValue typeVal = docObj.value(QLatin1String("type"));
    if (typeVal.isNull() || !typeVal.isString())
        return;
    if (typeVal.toString().compare(QLatin1String("login"), Qt::CaseInsensitive) == 0) { //It's a login message
        if (m_loggedIn)
            return;
        const QJsonValue resultVal = docObj.value(QLatin1String("success"));
        if (resultVal.isNull() || !resultVal.isBool())
            return;
        const bool loginSuccess = resultVal.toBool();
        if (loginSuccess) {

            emit loggedIn();
            return;
        }

        const QJsonValue reasonVal = docObj.value(QLatin1String("reason"));
        emit loginError(reasonVal.toString());
    } else if (typeVal.toString().compare(QLatin1String("message"), Qt::CaseInsensitive) == 0) { //It's a chat message
        const QJsonValue textVal = docObj.value(QLatin1String("text"));
        const QJsonValue senderVal = docObj.value(QLatin1String("sender"));
        if (textVal.isNull() || !textVal.isString())
            return;
        if (senderVal.isNull() || !senderVal.isString())
            return;
        emit messageReceived(senderVal.toString(), textVal.toString());
    } else if (typeVal.toString().compare(QLatin1String("newuser"), Qt::CaseInsensitive) == 0) { // A user joined the chat
        const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
        if (usernameVal.isNull() || !usernameVal.isString())
            return;
        emit userJoined(usernameVal.toString());
    } else if (typeVal.toString().compare(QLatin1String("userdisconnected"), Qt::CaseInsensitive) == 0) { // A user left the chat

        const QJsonValue usernameVal = docObj.value(QLatin1String("username"));
        if (usernameVal.isNull() || !usernameVal.isString())
            return;
        emit userLeft(usernameVal.toString());
    }
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
                if (jsonDoc.isObject())
                    jsonReceived(jsonDoc.object());
            }
        } else {

            break;
        }
    }
}

void Client::setPozicija_TenkaX(float pozicija_tenka_x, float pozicija_tenka_y)
{
    //connect(m_clientSocket, &Client::sendMessage, this, &Client::sendMessage);

    m_pozicija_tenka_x = pozicija_tenka_x;
    m_pozicija_tenka_y = pozicija_tenka_y;
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

