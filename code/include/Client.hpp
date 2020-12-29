#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
class QHostAddress;
class QJsonDocument;
class Client : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Client)
public:
    explicit Client(QObject *parent = nullptr);

//    void setPozicija_TenkaX(float pozicija_tenka_x);
//    void setPozicija_TenkaY(float pozicija_tenka_y);

//    float getPozicija_TenkaY();
//    float getPozicija_TenkaX();


    float x_Primljeno;
    float y_Primljeno;
    bool orderedToShoot;

//    static float getX_Primljeno();
//    static float getY_Primljeno();

    static bool isOrderedToShoot();
    static void setCantShoot();

    static qint16 getMovement();
    static void nullifyMovement();

public slots:
    void connectToServer(const QHostAddress &address, quint16 port);
    void login(const QString &userName);
    void sendMessage(const QString &text);
    void disconnectFromHost();
private slots:
    void onReadyRead();
signals:
    //void sendMessage(const QString &text);
    void connected();
    void loggedIn();
    void loginError(const QString &reason);
    void disconnected();
    void messageReceived(const QString &sender, const QString &text);
    void error(QAbstractSocket::SocketError socketError);
    void userJoined(const QString &username);
    void userLeft(const QString &username);
private:
    QTcpSocket *m_clientSocket;
    bool m_loggedIn;
    void jsonReceived(const QJsonObject &doc);
    float m_pozicija_tenka_x;
    float m_pozicija_tenka_y;
};
#endif // CLIENT_HPP
