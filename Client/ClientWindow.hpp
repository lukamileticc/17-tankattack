#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QWidget>
#include <QAbstractSocket>
class Client;
class QStandardItemModel;
namespace Ui { class ClientWindow; }
class ClientWindow : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(ClientWindow)
public:
    explicit ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();
private:
    Ui::ClientWindow *ui;
    Client *m_Client;
    QStandardItemModel *m_chatModel;
    QString m_lastUserName;
private slots:
    void attemptConnection();
    void connectedToServer();
    void attemptLogin(const QString &userName);
    void loggedIn();
    void loginFailed(const QString &reason);
    void messageReceived(const QString &sender, const QString &text);
    void sendMessage();
    void disconnectedFromServer();
    void userJoined(const QString &username);
    void userLeft(const QString &username);
    void error(QAbstractSocket::SocketError socketError);
};

#endif // MAINWINDOW_HPP
