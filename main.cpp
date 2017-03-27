#include "mytcpclient.h"
#include <QApplication>
#include <QtNetwork>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QtXml>
#include <QDateTime>
#include <QThread>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "mytcpclient.h"







int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MyTcpClient client;
    client.show();
    return app.exec();
}
