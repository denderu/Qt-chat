#include "serverview.h"
#include "ui_serverview.h"

#include <QDialog>
#include <QMessageBox>

ServerView::ServerView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerView)
{
    ui->setupUi(this);
    server = new ServerModel();

    ui->stopButton->setEnabled(false);

    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(OnStartButton()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(OnStopButton()));
    connect(server, SIGNAL(DataReceived(QString)), this, SLOT(UpdateChat(QString)));
    connect(server, SIGNAL(UpdateUsers(QString)), this, SLOT(UpdateUsersList(QString)));
}

ServerView::~ServerView()
{
    delete ui;
    delete server;
}

void ServerView::OnStartButton()
{
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    ui->ipBox->setEnabled(false);
    ui->portBox->setEnabled(false);
    //QHostAddress ip = QHostAddress(ui->ipBox->text());
    //qint32 port = ui->portBox->text().toInt();
    server->Start(QHostAddress::Any, 1111);
}

void ServerView::OnStopButton()
{
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    ui->ipBox->setEnabled(true);
    ui->portBox->setEnabled(true);
    server->Stop();
}

void ServerView::UpdateChat(const QString & msg)
{
    m_chat += msg + "\n";
    ui->chatLabel->setText(m_chat);
}

void ServerView::UpdateUsersList(QString usersList)
{
    ui->usersLabel->setText(usersList);
}


