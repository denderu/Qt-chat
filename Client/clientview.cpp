#include "clientview.h"
#include "ui_clientview.h"

#include <QMessageBox>
#include <QHostAddress>

ClientView::ClientView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientView)
{
    ui->setupUi(this);
    client = new ClientModel();
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(OnConnectButton()));
    connect(ui->disconnectButton, SIGNAL(clicked()), this, SLOT(OnDisconnectButton()));
    connect(ui->sendButton, SIGNAL(clicked()), this, SLOT(Send()));
    connect(ui->messageBox, SIGNAL(returnPressed()), this, SLOT(Send()));
    connect(client, SIGNAL(connected()), this, SLOT(Connected()));
    connect(client, SIGNAL(disconnected()), this, SLOT(Disconnected()));
    connect(client, SIGNAL(DataReceived(QString)), this, SLOT(UpdateChat(QString)));
}

ClientView::~ClientView()
{
    delete ui;
    delete client;
}

void ClientView::OnConnectButton()
{
    if (ui->usernameBox->text().isEmpty())
    {
        QMessageBox::critical(0, "Client", "Please, enter the username");
        return;
    }

    if (ui->usernameBox->text().size() > 8)
    {
        QMessageBox::critical(0, "Client", "Max characters of username: 8");
        return;
    }

    //QString ip = ui->ipBox->text();
    //QString port = ui->portBox->text();

    client->Connect("127.0.0.1", "1111");
}

void ClientView::OnDisconnectButton()
{
    client->Disconnect();
}

void ClientView::Send()
{
    if (ui->usernameBox->text().isEmpty())
        return;

    QString str = ui->usernameBox->text() + ": " + ui->messageBox->text();
    client->Write(str);
    ui->messageBox->clear();
}

void ClientView::Connected()
{
    ui->ipBox->setEnabled(false);
    ui->portBox->setEnabled(false);
    ui->usernameBox->setEnabled(false);

    ui->connectButton->setEnabled(false);
    ui->disconnectButton->setEnabled(true);
    ui->sendButton->setEnabled(true);

    client->Write(ui->usernameBox->text());
}

void ClientView::Disconnected()
{
    ui->ipBox->setEnabled(true);
    ui->portBox->setEnabled(true);
    ui->usernameBox->setEnabled(true);

    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->sendButton->setEnabled(false);
}

void ClientView::UpdateChat(const QString & msg)
{
    m_chat += msg + "\n";
    ui->chatBox->setText(m_chat);
}

