#ifndef SERVERVIEW_H
#define SERVERVIEW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include "servermodel.h"

namespace Ui {
class ServerView;
}

class ServerView : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerView(QWidget *parent = 0);
    ~ServerView();

public slots:
    void OnStartButton();
    void OnStopButton();
    void UpdateChat(const QString & msg);
    void UpdateUsers(const QString & nickname);

private:
    Ui::ServerView *ui;
    ServerModel *server;
    QString m_chat;
    QString m_users;
};

#endif // SERVERVIEW_H
