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

public:
    explicit ServerModel();
    ~ServerModel();

public slots:
    void Start(QHostAddress ip, qint32 port);
    void Stop();
    void NewConnection();
    void Read();
    void Write(QTcpSocket * socket, const QString & str);

signals:
    void DataReceived(const QString & message);
    void NickNameReceived(const QString & nickname);

private:
    quint16 m_nextBlockSize;
    bool m_firstMessage;

};

#endif // SERVERMODEL_H
