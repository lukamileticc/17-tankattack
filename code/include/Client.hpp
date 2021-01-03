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

    void setTanksX(float pozicija_tenka_x);
    void setTanksY(float pozicija_tenka_y);
    void setTanksAngle(float angle);

    float getTankX();
    float getTankY();

    static float getReceivedX();
    static float getReceivedY();
    static float getReceivedAngle();

    static bool isOrderedToShoot();
    static void setCantShoot();

public slots:
    void connectToServer(const QHostAddress &address, quint16 port);
    void login(const QString &userName);
    void sendMessage(const QString &text);
    void disconnectFromHost();
private slots:
    void onReadyRead();
signals:
    void connected();
    void loggedIn();
    void loginError(const QString &reason);
    void disconnected();
    void messageReceived(const QString &sender, const QString &text);
    void error(QAbstractSocket::SocketError socketError);
private:
    QTcpSocket *m_clientSocket;
    bool m_loggedIn;
    void jsonReceived(const QJsonObject &doc);
    float m_TankX;
    float m_TankY;
    float m_angle;
};
#endif // CLIENT_HPP
