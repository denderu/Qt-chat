#include <QMessageBox>
#include <QDebug>
#include <QTime>

#include "servermodel.h"

ServerModel::ServerModel() : m_nextBlockSize(0)
{
    connect(this, SIGNAL(newConnection()), this, SLOT(NewConnection()));
}

ServerModel::~ServerModel()
{
   this->close();
}

void ServerModel::Start(QHostAddress ip, qint32 port)
{
    if (port < 1024 || port > 65535)
    {
       QMessageBox::critical(0, "Server Error", "Invalid port");
       this->close();
       return;
    }

    if(!this->listen(ip, port))
    {
       QMessageBox::critical(0, "Server Error", "Start failed. " + this->errorString());
       this->close();
       return;
    }
}

void ServerModel::Stop()
{
    this->close();
}

void ServerModel::NewConnection()
{
    QTcpSocket *sock = new QTcpSocket();
    sock = this->nextPendingConnection();

    m_firstMessage = true;

    connect(sock, SIGNAL(readyRead()), this, SLOT(Read()));
    connect(sock, SIGNAL(disconnected()), this, SLOT(UserDisconnected()));
}

void ServerModel::UserDisconnected()
{
    QString message;
    VectorIterator it;
    QTcpSocket* socket = qobject_cast<QTcpSocket *>(sender());

    message = "*** " + m_clients[socket] + " has left the chat***";
    emit DataReceived(message);

    it = std::find(m_users.begin(), m_users.end(), m_clients[socket]);
    m_users.erase(it);

    UpdateUsersList();

    m_clients.erase(socket);
    socket->deleteLater();
}

void ServerModel::Read()
{
    QTime time;
    QString str, message, nicknames;
    MapIterator itMap;
    VectorIterator itVec;
    QTcpSocket* socket = qobject_cast<QTcpSocket *>(sender());
    QDataStream readStream(socket);

    while(true)
    {
        if(!m_nextBlockSize)
        {
            if(socket->bytesAvailable() < sizeof(quint16))
                break;
            readStream >> m_nextBlockSize;
        }

        if(socket->bytesAvailable() < m_nextBlockSize)
            break;

        readStream >> time >> str;

        if(m_firstMessage)
        {
            m_firstMessage = false;

            m_clients[socket] = str;
            m_users.push_back(str);

            UpdateUsersList();

            message = "*** " + str + " enter the chat***";
            emit DataReceived(message);
            return;
        }

        message = "[" + time.toString() + "] " + str;

        for (itMap= m_clients.begin(); itMap != m_clients.end(); itMap++)
            Write(itMap->first, message);

        emit DataReceived(message);
        m_nextBlockSize = 0;
    }
}

void ServerModel::Write(QTcpSocket * socket, const QString & str)
{
    QByteArray block;
    QDataStream sendStream(&block, QIODevice::WriteOnly);
    sendStream << quint16(0) << QTime::currentTime() << str;

    sendStream.device()->seek(0);
    sendStream << quint16(block.size() - sizeof(quint16));

    socket->write(block);
}

void ServerModel::UpdateUsersList()
{
    QString nicknames;
    VectorIterator it;
    for (it= m_users.begin(); it != m_users.end(); it++)
        nicknames += *it + "\n";
    emit UpdateUsers(nicknames);
}

