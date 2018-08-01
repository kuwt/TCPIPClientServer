#ifndef COMMUNWIN_H
#define COMMUNWIN_H

#include <QObject>
#include <QWidget>
#include <QtNetwork>
#include <QString>

class CommunWin : public QWidget
{
    Q_OBJECT

signals:
    void MsgSignal(QString);

public:
    explicit CommunWin(QWidget *parent = 0);
    ~CommunWin();
public slots:
    void Start();
    void sendClient(QString msg);

private slots:
    void acceptConnection();
    void readClient();

private:
    QTcpServer *server;
    QTcpSocket *clientConnection;
};

#endif // COMMUNWIN_H
