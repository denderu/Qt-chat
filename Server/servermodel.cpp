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
}

void ServerModel::Read()
{
    QTime time;
    QString str;
    QString message;
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
        {
            break;
        }

        readStream >> time >> str;

        if(m_firstMessage)
        {
            m_firstMessage = false;
            emit NickNameReceived(str);

            message = "***[" + time.toString() + "] " + str + " enter the chat***";
            emit DataReceived(message);
            return;
        }

        message = "[" + time.toString() + "] " + str;
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

