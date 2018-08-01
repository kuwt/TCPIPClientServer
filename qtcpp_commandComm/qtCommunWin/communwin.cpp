#include "communwin.h"
#include <QByteArray>
#include <QDebug>
CommunWin::CommunWin(QWidget *parent) :
    QWidget(parent)
{
    qDebug() << "CommunWin";
    server = new QTcpServer();
    clientConnection = 0;
}

CommunWin::~CommunWin()
{

}

void CommunWin::Start()
{
     qDebug() << "Start";
    server->listen(QHostAddress::Any, 6665);
    connect(server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

void CommunWin::sendClient(QString msg)
{
      qDebug() << "sendClient";

      if (clientConnection && clientConnection->state() == QAbstractSocket::ConnectedState)
      {
        clientConnection->write( msg.toStdString().c_str());
      }
}

void CommunWin::acceptConnection()
{
    qDebug() << "acceptConnection";
    clientConnection = server->nextPendingConnection();

    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readClient()));
}
void CommunWin::readClient()
{
     qDebug() << "readClient";
    char buf[1024] = "";
    clientConnection->read(buf,1024);
    QString str = buf;
qDebug() << str;
    emit MsgSignal(str);
}
