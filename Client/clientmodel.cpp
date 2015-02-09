#include <QTime>
#include <QHostAddress>

#include "clientmodel.h"

ClientModel::ClientModel(): m_nextBlockSize(0)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(Read()));
}

ClientModel::~ClientModel()
{
   this->close();
}

void ClientModel::Connect(QString ipaddr, QString portaddr)
{
    QHostAddress ip = QHostAddress(ipaddr);
    int port = portaddr.toInt();

    this->connectToHost(ip, port);
}

void ClientModel::Disconnect()
{
    this->disconnectFromHost();
}

void ClientModel::Read()
{
    QTime time;
    QString str;
    QDataStream readStream(this);
    while(true)
    {
        if(!m_nextBlockSize)
        {
            if(this->bytesAvailable() < sizeof(quint16))
                break;
            readStream >> m_nextBlockSize;
        }

        if(this->bytesAvailable() < m_nextBlockSize)
        {
            break;
        }

        readStream >> time >> str;

        emit DataReceived(str);
        m_nextBlockSize = 0;
    }
}

void ClientModel::Write(const QString & str)
{
    QByteArray block;
    QDataStream sendStream(&block, QIODevice::WriteOnly);
    sendStream << quint16(0) << QTime::currentTime() << str;

    sendStream.device()->seek(0);
    sendStream << quint16(block.size() - sizeof(quint16));
    this->write(block);
}

