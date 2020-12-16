#ifndef SERVERWINDOW_HPP
#define SERVERWINDOW_HPP

#include <QWidget>

namespace Ui {
class ServerWindow;
}
class Server;
class ServerWindow : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(ServerWindow)
public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private:
    Ui::ServerWindow *ui;
    Server *m_Server;
private slots:
    void toggleStartServer();
    void logMessage(const QString &msg);
};

#endif // SERVERWINDOW_HPP
