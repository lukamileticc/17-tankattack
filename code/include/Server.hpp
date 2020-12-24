#ifndef SERVER_HPP
#define SERVER_HPP

#include <QTcpServer>
#include <QVector>

class QThread;
class ServerWorker;
class QJsonObject;
class Server : public QTcpServer
{
    Q_OBJECT
    Q_DISABLE_COPY(Server)

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();
    float x_new;
    float y_new;

    static float getX();
    static float getY();

protected:
    void incomingConnection(qintptr socketDescriptor) override;
private:
    const int m_idealThreadCount;
    QVector<QThread *> m_availableThreads;
    QVector<int> m_threadsLoad;
    QVector<ServerWorker *> m_clients;
private slots:
    void broadcast(const QJsonObject &message, ServerWorker *exclude);
    void jsonReceived(ServerWorker *sender, const QJsonObject &doc);
    void userDisconnected(ServerWorker *sender, int threadIdx);
    void userError(ServerWorker *sender);
public slots:
    void stopServer();
private:
    void sendJson(ServerWorker *destination, const QJsonObject &message);
signals:
    void logMessage(const QString &msg);
    void stopAllClients();
};


#endif // SERVER_HPP
