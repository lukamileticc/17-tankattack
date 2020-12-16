#include "ClientWindow.hpp"
#include "ui_ClientWindow.h"

#include "include/Client.hpp"

#include <QStandardItemModel>
#include <QInputDialog>
#include <QMessageBox>
#include <QHostAddress>
ClientWindow::ClientWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientWindow)
    , m_Client(new Client(this))
    , m_chatModel(new QStandardItemModel(this))
{
    ui->setupUi(this);
    m_chatModel->insertColumn(0);
    ui->chatView->setModel(m_chatModel);
    connect(m_Client, &Client::connected, this, &ClientWindow::connectedToServer);
    connect(m_Client, &Client::loggedIn, this, &ClientWindow::loggedIn);
    connect(m_Client, &Client::loginError, this, &ClientWindow::loginFailed);
    connect(m_Client, &Client::messageReceived, this, &ClientWindow::messageReceived);
    connect(m_Client, &Client::disconnected, this, &ClientWindow::disconnectedFromServer);
    connect(m_Client, &Client::error, this, &ClientWindow::error);
    connect(m_Client, &Client::userJoined, this, &ClientWindow::userJoined);
    connect(m_Client, &Client::userLeft, this, &ClientWindow::userLeft);
    connect(ui->connectButton, &QPushButton::clicked, this, &ClientWindow::attemptConnection);
    connect(ui->sendButton, &QPushButton::clicked, this, &ClientWindow::sendMessage);
    connect(ui->messageEdit, &QLineEdit::returnPressed, this, &ClientWindow::sendMessage);
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::attemptConnection()
{
    const QString hostAddress = QInputDialog::getText(
        this
        , tr("Chose Server")
        , tr("Server Address")
        , QLineEdit::Normal
        , QStringLiteral("127.0.0.1")
    );
    if (hostAddress.isEmpty())
        return;
    ui->connectButton->setEnabled(false);
    m_Client->connectToServer(QHostAddress(hostAddress), 1967);
}

void ClientWindow::connectedToServer()
{
    const QString newUsername = QInputDialog::getText(this, tr("Chose Username"), tr("Username"));
    if (newUsername.isEmpty()){
        return m_Client->disconnectFromHost();
    }
    attemptLogin(newUsername);
}

void ClientWindow::attemptLogin(const QString &userName)
{
    m_Client->login(userName);
}

void ClientWindow::loggedIn()
{
    ui->sendButton->setEnabled(true);
    ui->messageEdit->setEnabled(true);
    ui->chatView->setEnabled(true);
    m_lastUserName.clear();
}

void ClientWindow::loginFailed(const QString &reason)
{
    QMessageBox::critical(this, tr("Error"), reason);
    connectedToServer();
}

void ClientWindow::messageReceived(const QString &sender, const QString &text)
{
    int newRow = m_chatModel->rowCount();
    if (m_lastUserName != sender) {
        m_lastUserName = sender;
        QFont boldFont;
        boldFont.setBold(true);
        m_chatModel->insertRows(newRow, 2);
        m_chatModel->setData(m_chatModel->index(newRow, 0), sender + QLatin1Char(':'));
        m_chatModel->setData(m_chatModel->index(newRow, 0), int(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextAlignmentRole);
        m_chatModel->setData(m_chatModel->index(newRow, 0), boldFont, Qt::FontRole);
        ++newRow;
    } else {
        m_chatModel->insertRow(newRow);
    }
    m_chatModel->setData(m_chatModel->index(newRow, 0), text);
    m_chatModel->setData(m_chatModel->index(newRow, 0), int(Qt::AlignLeft | Qt::AlignVCenter), Qt::TextAlignmentRole);
    ui->chatView->scrollToBottom();
}

void ClientWindow::sendMessage()
{
    m_Client->sendMessage(ui->messageEdit->text());

    const int newRow = m_chatModel->rowCount();
    m_chatModel->insertRow(newRow);
    m_chatModel->setData(m_chatModel->index(newRow, 0), ui->messageEdit->text());
    m_chatModel->setData(m_chatModel->index(newRow, 0), int(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
    ui->messageEdit->clear();
    ui->chatView->scrollToBottom();
    m_lastUserName.clear();
}

void ClientWindow::disconnectedFromServer()
{

    QMessageBox::warning(this, tr("Disconnected"), tr("The host terminated the connection"));
    ui->sendButton->setEnabled(false);
    ui->messageEdit->setEnabled(false);
    ui->chatView->setEnabled(false);
    ui->connectButton->setEnabled(true);
    m_lastUserName.clear();
}

void ClientWindow::userJoined(const QString &username)
{
    const int newRow = m_chatModel->rowCount();
    m_chatModel->insertRow(newRow);
    m_chatModel->setData(m_chatModel->index(newRow, 0), tr("%1 Joined the Chat").arg(username));
    m_chatModel->setData(m_chatModel->index(newRow, 0), Qt::AlignCenter, Qt::TextAlignmentRole);
    m_chatModel->setData(m_chatModel->index(newRow, 0), QBrush(Qt::blue), Qt::ForegroundRole);
    ui->chatView->scrollToBottom();
    m_lastUserName.clear();
}
void ClientWindow::userLeft(const QString &username)
{
    const int newRow = m_chatModel->rowCount();
    m_chatModel->insertRow(newRow);
    m_chatModel->setData(m_chatModel->index(newRow, 0), tr("%1 Left the Chat").arg(username));
    m_chatModel->setData(m_chatModel->index(newRow, 0), Qt::AlignCenter, Qt::TextAlignmentRole);
    m_chatModel->setData(m_chatModel->index(newRow, 0), QBrush(Qt::red), Qt::ForegroundRole);
    ui->chatView->scrollToBottom();
    m_lastUserName.clear();
}

void ClientWindow::error(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
    case QAbstractSocket::ProxyConnectionClosedError:
        return;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::critical(this, tr("Error"), tr("The host refused the connection"));
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        QMessageBox::critical(this, tr("Error"), tr("The proxy refused the connection"));
        break;
    case QAbstractSocket::ProxyNotFoundError:
        QMessageBox::critical(this, tr("Error"), tr("Could not find the proxy"));
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::critical(this, tr("Error"), tr("Could not find the server"));
        break;
    case QAbstractSocket::SocketAccessError:
        QMessageBox::critical(this, tr("Error"), tr("You don't have permissions to execute this operation"));
        break;
    case QAbstractSocket::SocketResourceError:
        QMessageBox::critical(this, tr("Error"), tr("Too many connections opened"));
        break;
    case QAbstractSocket::SocketTimeoutError:
        QMessageBox::warning(this, tr("Error"), tr("Operation timed out"));
        return;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        QMessageBox::critical(this, tr("Error"), tr("Proxy timed out"));
        break;
    case QAbstractSocket::NetworkError:
        QMessageBox::critical(this, tr("Error"), tr("Unable to reach the network"));
        break;
    case QAbstractSocket::UnknownSocketError:
        QMessageBox::critical(this, tr("Error"), tr("An unknown error occured"));
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        QMessageBox::critical(this, tr("Error"), tr("Operation not supported"));
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        QMessageBox::critical(this, tr("Error"), tr("Your proxy requires authentication"));
        break;
    case QAbstractSocket::ProxyProtocolError:
        QMessageBox::critical(this, tr("Error"), tr("Proxy comunication failed"));
        break;
    case QAbstractSocket::TemporaryError:
    case QAbstractSocket::OperationError:
        QMessageBox::warning(this, tr("Error"), tr("Operation failed, please try again"));
        return;
    default:
        Q_UNREACHABLE();
    }
    ui->connectButton->setEnabled(true);
    ui->sendButton->setEnabled(false);
    ui->messageEdit->setEnabled(false);
    ui->chatView->setEnabled(false);
    m_lastUserName.clear();
}
