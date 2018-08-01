#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void Connect();
    void SendMsg();
    void readClient();
    void SendInitScanner();
    void SendTriggerPicture();
    void SendCaptureTarget();
    void SendCaptureTargetError();
    void SendCaptureTargetResponseSuccess();
    void SendCaptureTargetResponseFail();

    void SendGetAllTemplate();
    void SendGetSelectedTemplate();
    void SendClearTemplate();
    void SendAddTemplate();
    void SendDeleteTemplate();
private:
    Ui::MainWindow *ui;
    QTcpSocket *m_client;


};

#endif // MAINWINDOW_H
