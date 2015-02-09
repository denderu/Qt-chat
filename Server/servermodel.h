#ifndef SERVERMODEL_H
#define SERVERMODEL_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTcpServer>
#include <QTcpSocket>

class ServerModel : public QTcpServer
{
    Q_OBJECT

    typedef std::map<QTcpSocket *, QString>::iterator MapIterator;
    typedef std::vector<QString>::iterator VectorIterator;

public:
    explicit ServerModel();
    ~ServerModel();
    void UpdateUsersList();

public slots:
    void Start(QHostAddress ip, qint32 port);
    void Stop();
    void NewConnection();
    void UserDisconnected();
    void Read();
    void Write(QTcpSocket * socket, const QString & str);

signals:
    void DataReceived(const QString & message);
    void UpdateUsers(QString usersList);

private:
    quint16 m_nextBlockSize;
    bool m_firstMessage;
    std::vector<QString> m_users;
    std::map <QTcpSocket *, QString> m_clients;

};

#endif // SERVERMODEL_H
