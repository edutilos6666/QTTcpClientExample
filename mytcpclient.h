#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QWidget>
#include "ui_mytcpclient.h"
#include <QMainWindow>
class MyTcpClient: public QMainWindow
{
    Q_OBJECT
public:
    MyTcpClient(QWidget *parent = 0);
    ~MyTcpClient();

    void registerEvents();
    void connect();

public slots:
    void startChat();
    void sendMessage();
    void closeChat();
    void onBytesWritten(qint64 nobytes);
    void receiveMessage();
public:
    QString clientId ;
    QString username;
    bool firstTime = true;
    QTcpSocket *client;
     Ui::MyTcpClient *ui;



};

#endif // MYTCPCLIENT_H
