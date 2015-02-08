#ifndef CLIENTVIEW_H
#define CLIENTVIEW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

#include "clientmodel.h"

namespace Ui {
class ClientView;
}

class ClientView : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientView(QWidget *parent = 0);
    ~ClientView();

public slots:
    void OnConnectButton();
    void OnDisconnectButton();
    void Send();
    void Receive();
    void Connected();
    void Disconnected();

private:
    Ui::ClientView *ui;
    ClientModel *client;
};

#endif // CLIENTVIEW_H
