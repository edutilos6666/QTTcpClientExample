#include "mytcpclient.h"
#include "ui_mytcpclient.h"





#include "mytcpclient.h"
#include <QtNetwork>

MyTcpClient::MyTcpClient(QWidget *parent):
QMainWindow(parent), ui(new Ui::MyTcpClient)
{
    ui->setupUi(this);
  client = new QTcpSocket();
  registerEvents();
}

MyTcpClient::~MyTcpClient()
{
   delete ui;
}



void MyTcpClient::registerEvents() {


   QObject::connect(client, SIGNAL(readyRead()), this, SLOT(receiveMessage()));
   //QObject::connect(client , SIGNAL(readyRead()), this, SLOT(sendMessage()));

//    QObject::connect(client , &QTcpSocket::readyRead, [this](){
//        this->receiveMessage();
//    });
//    QObject::connect(client , &QTcpSocket::bytesWritten, [](){
//     //onBytesWritten(0);

//    });


    QObject::connect(client, &QTcpSocket::connected, [](){
         qDebug() << "connected";

    });

    QObject::connect(client , &QTcpSocket::disconnected, [](){
        qDebug() << "disconnected";
    });


    QObject::connect(ui->btnStart , SIGNAL(clicked()),this, SLOT(startChat()));
     QObject::connect(ui->btnSend , SIGNAL(clicked()),this, SLOT(sendMessage()));
     QObject::connect(ui->btnClose , SIGNAL(clicked()),this, SLOT(closeChat()));

//    QObject::connect(ui->btnSend , &QPushButton::clicked, [this](){
//        sendMessage();
//    });

//    QObject::connect(ui->btnClose , &QPushButton::clicked, [this](){
//        closeChat();
//    });


}

/*<?xml version="1.0"?>
<ChatServerRequest timestamp="2014-08-07T11:40:45" clientId="usjhg23" service="startChat">
<Parameters>
<Param>MarcT</Param>
</Parameters>
</ChatServerRequest>
 * */

void MyTcpClient::onBytesWritten(qint64 nobytes) {
    //client->disconnectFromHost();
   // delete client;
}

void MyTcpClient::connect() {
    if(client->state() ==  QAbstractSocket::SocketState::ConnectedState)
        client->disconnectFromHost();
 // client = new QTcpSocket();
 // registerEvents();


//    QString host = "134.147.194.159";
//    int port = 9911;

  QString host = ui->editHost->text();
  int port = ui->editPort->text().toInt();
    client->connectToHost(host , port);
    username = ui->editUsername->text();
    client->waitForConnected();
}



void MyTcpClient::startChat() {
   connect();
 client->waitForReadyRead();
   // qDebug() << "inside startChat = " << client->state() << " " << clientId;
QString xmlOutput;
QXmlStreamWriter writer(&xmlOutput) ;
writer.writeStartDocument();
writer.writeStartElement("ChatServerRequest");
//QString tempClientId = clientId ,
//        tempUsername = username;
writer.writeAttribute("timestamp", QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate));
writer.writeAttribute("clientId", clientId);
writer.writeAttribute("service", "startChat");
writer.writeStartElement("Parameters");
writer.writeTextElement("Param", username);
writer.writeEndElement();
writer.writeEndElement();
writer.writeEndDocument();

//qDebug() << xmlOutput;
/*
client->write(xmlOutput.toUtf8())*/;
qDebug() << "inside startChat" ;
QByteArray arr ;
QDataStream out(&arr, QIODevice::ReadWrite);
out << (quint16)0;
out << xmlOutput;
out.device()->seek(0);
out << (quint16)(xmlOutput.size()- sizeof(quint16));
client->write(arr);
//client->waitForBytesWritten();

}


/*
 * <?xml version="1.0"?>
<ChatServerRequest timestamp="2014-08-07T11:40:45" clientId="usjhg23" service="sendMessage">
<Parameters>
<Param>MarcT</Param>
<Param>Die ist ein Test!</Param>
</Parameters>
</ChatServerRequest>
 * */
void MyTcpClient::sendMessage() {
    connect();
    client->waitForReadyRead();
//    QString tempClientId = clientId ,
//            tempUsername = username;
    qDebug() << "inside sendMessage "  << client->state() << " " << clientId;
    QString xmlOutput;
    QXmlStreamWriter writer(&xmlOutput) ;
   writer.writeStartDocument();
   writer.writeStartElement("ChatServerRequest");
   writer.writeAttribute("timestamp", QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate));
   writer.writeAttribute("clientId", clientId);
   writer.writeAttribute("service","sendMessage");
   writer.writeStartElement("Parameters");
   writer.writeTextElement("Param", username);
   QString msg = ui->editMessage->document()->toPlainText();
   writer.writeTextElement("Param", msg);
   writer.writeEndElement();
   writer.writeEndElement();
   writer.writeEndDocument();

   QByteArray arr ;
   QDataStream out(&arr, QIODevice::ReadWrite);
  out << (quint16)0;
   out << xmlOutput;
  out.device()->seek(0);
   out << (quint16)(xmlOutput.size()- sizeof(quint16));
   client->write(arr);
   //client->waitForBytesWritten();

}


/*
 * <?xml version="1.0"?>
<ChatServerRequest timestamp="2014-08-07T11:40:45" clientId="usjhg23" service="closeChat">
<Parameters/>
</ChatServerRequest>
 * */

void MyTcpClient::closeChat() {
//    QString tempClientId = clientId ,
//            tempUsername = username;
    connect();
     client->waitForReadyRead();
    qDebug() << "inside closeChat " << client->state() << " " << clientId;
    QString xmlOutput;
    QXmlStreamWriter writer(&xmlOutput) ;
writer.writeStartDocument();
writer.writeStartElement("ChatServerRequest");
 writer.writeAttribute("timestamp", QDateTime::currentDateTime().toString(Qt::DateFormat::ISODate));
 writer.writeAttribute("clientId", clientId);
 writer.writeAttribute("service", "closeChat");
 writer.writeStartElement("Parameters");
writer.writeEndElement();
writer.writeEndDocument();



QByteArray arr ;
QDataStream out(&arr, QIODevice::ReadWrite);
 out << (quint16)0;
out << xmlOutput;
 out.device()->seek(0);
out << (quint16)(xmlOutput.size()- sizeof(quint16));
client->write(arr);
//client->waitForBytesWritten();

}


void MyTcpClient::receiveMessage() {

  //  qDebug() << "inside reciveMessage";
  quint16 blocksize ;
  QString message;
  QDataStream in(client);
 //  qDebug() << client->bytesAvailable();
 // if(client->bytesAvailable() < sizeof(quint16)) return ;
  in >> blocksize ;
 // if(client->bytesAvailable() < blocksize) return ;
  in >> message;

  QXmlStreamReader reader(message);
  reader.readNextStartElement();
   if(reader.name() == "ChatServerConnected") {

       QXmlStreamAttributes &attrs = reader.attributes();
       QString tempClientId = attrs.value("clientId").toString();
       if(firstTime && tempClientId != "") {
           firstTime = false;
 clientId = tempClientId;
 //clientId = clientId.mid(1, clientId.count()-1);
       }
    // qDebug() << "clientId = " << clientId ;
   }
  qDebug() << blocksize << " and " << message ;
  QString out ;
  QTextStream stream(&out);
  stream << blocksize << " and " << message ;
  ui->editOutput->document()->setPlainText(out);
}
