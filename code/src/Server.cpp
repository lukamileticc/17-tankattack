#include "../include/Server.hpp"
#include "../include/ServerWorker.hpp"

#include <QThread>
#include <functional>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTimer>


float x_tmp;
float y_tmp;
bool  orderedToShoot_tmp;
int numOfClients = 0;
bool isStart = false;
Server::Server(QObject *parent)
    : QTcpServer(parent)
    , m_idealThreadCount(qMax(QThread::idealThreadCount(), 1))
{
    m_availableThreads.reserve(m_idealThreadCount);
    m_threadsLoad.reserve(m_idealThreadCount);
}

Server::~Server()
{
    for (QThread *singleThread : m_availableThreads) {
        singleThread->quit();
        singleThread->wait();
    }
}
void Server::incomingConnection(qintptr socketDescriptor)
{
    ServerWorker *worker = new ServerWorker;
    if (!worker->setSocketDescriptor(socketDescriptor)) {
        worker->deleteLater();
        return;
    }
    int threadIdx = m_availableThreads.size();
    if (threadIdx < m_idealThreadCount) {
        m_availableThreads.append(new QThread(this));
        m_threadsLoad.append(1);
        m_availableThreads.last()->start();
    } else {
        threadIdx = std::distance(m_threadsLoad.cbegin(), std::min_element(m_threadsLoad.cbegin(), m_threadsLoad.cend()));
        ++m_threadsLoad[threadIdx];
    }
    worker->moveToThread(m_availableThreads.at(threadIdx));
    connect(m_availableThreads.at(threadIdx), &QThread::finished, worker, &QObject::deleteLater);
    connect(worker, &ServerWorker::disconnectedFromClient, this, std::bind(&Server::userDisconnected, this, worker, threadIdx));
    connect(worker, &ServerWorker::error, this, std::bind(&Server::userError, this, worker));
    connect(worker, &ServerWorker::jsonReceived, this, std::bind(&Server::jsonReceived, this, worker, std::placeholders::_1));
    connect(this, &Server::stopAllClients, worker, &ServerWorker::disconnectFromClient);
    m_clients.append(worker);

    if(m_clients.size() == 2)
    {
        QJsonObject message = {
            {"Start", 1}
        };

        broadcast(message, nullptr);
    }
    emit logMessage(QStringLiteral("New client Connected"));
}
void Server::sendJson(ServerWorker *destination, const QJsonObject &message)
{
    Q_ASSERT(destination);
    QTimer::singleShot(0, destination, std::bind(&ServerWorker::sendJson, destination, message));
}
void Server::broadcast(const QJsonObject &message, ServerWorker *exclude)
{

    qDebug() << m_clients.size() << "velicina";
    for (ServerWorker *worker : m_clients) {
        Q_ASSERT(worker);
        if (worker != exclude){
            qDebug() << "poslao";
        sendJson(worker, message);
        break;
        }
    }
}

void Server::jsonReceived(ServerWorker *sender, const QJsonObject &json)
{
    Q_ASSERT(sender);
    emit logMessage(QLatin1String("JSON received ") + QString::fromUtf8(QJsonDocument(json).toJson()));

    QJsonObject message;
    if(*json.find("Space") != *json.end())
    {
        message = {

            {"Space", *json.find("Space")}
        };
    }
    else
        message = json;

    broadcast(message, sender);
}

void Server::userDisconnected(ServerWorker *sender, int threadIdx)
{
    --m_threadsLoad[threadIdx];
    m_clients.removeAll(sender);
    sender->deleteLater();
}

void Server::userError(ServerWorker *sender)
{
    Q_UNUSED(sender)
    emit logMessage(QLatin1String("Error from "));
}

void Server::stopServer()
{
    emit stopAllClients();
    close();
}

float Server::getX()
{

   return x_tmp;
}
float Server::getY()
{
    return y_tmp;
}

int Server::getNumOfClients()
{
    return m_clients.size();
}


