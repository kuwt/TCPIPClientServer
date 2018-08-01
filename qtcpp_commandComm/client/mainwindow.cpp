#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QString>
#include <QInputDialog>
#define START_FLAG 0x01
#define MID_FLAG 0x09
#define END_FLAG 0x02

#define INIT_ROBOT_CMD 0x0A
#define INIT_SCANNER_CMD 0x51
#define TRIGGER_CMD 0x52
#define RECOGNITION_CMD 0x53

#define ROBOT_GRAB_RESPONSE_SUCCESS 0xAA
#define ROBOT_GRAB_RESPONSE_FAIL 0xAB
#define SYN_CMD 0xAA
#define MOVE_CMD 0x0B
#define HOME_CMD 0x0C
#define STOP_CMD 0x0D
#define SWEEP_CMD 0x0F
#define GET_STATUS_CMD 0x0E
#define GET_ALL_TEMPL_CMD 0x54
#define GET_SELECTED_TEMPL_CMD 0x55
#define CLEAR_ALL_TEMPL_CMD 0x56
#define ADD_TEMPL_CMD 0x57
#define DELETE_TEMPL_CMD 0x58

#define MAX_MSG_SIZE (1024)
inline std::string printMessage(std::string strMsg)
{
    std::string str;
    for (int i = 0; i < strMsg.size(); i++)
    {
        char buff[1024];

        snprintf(buff, sizeof(buff),"0x%02x ",strMsg[i]);
        str+= std::string(buff);
    }
    return str;
}

int GetUserInputLineEdit(QWidget *parent,
                         QString title,
                         QString info,
                         QString defaultInput,
                         QString &output)
{
    bool ok = false;
    QString text = QInputDialog::getText(parent,
                                         title,
                                         info,
                                         QLineEdit::Normal,
                                         defaultInput, &ok);
    if (ok && !text.isEmpty())
    {
        output = text;

    }
    else
    {
        output = "";
    }
    return ok;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_client = new QTcpSocket(this);

    connect(ui->pushButtonConnect, SIGNAL(clicked()), this, SLOT(Connect()));

    connect(m_client,SIGNAL(readyRead()), this, SLOT(readClient()));

    connect(ui->pushButtonInitScanner, SIGNAL(clicked()), this, SLOT(SendInitScanner()));
    connect(ui->pushButtonTriggerPicture, SIGNAL(clicked()), this, SLOT(SendTriggerPicture()));
    connect(ui->pushButtonCaptureTarget, SIGNAL(clicked()), this, SLOT(SendCaptureTarget()));
     connect(ui->pushButtonCaptureTargetError, SIGNAL(clicked()), this, SLOT(SendCaptureTargetError()));
     connect(ui->pushButtonCaptureTargetResponseSuccess, SIGNAL(clicked()), this, SLOT(SendCaptureTargetResponseSuccess()));
    connect(ui->pushButtonCaptureTargetResponseFail, SIGNAL(clicked()), this, SLOT(SendCaptureTargetResponseFail()));

    connect(ui->pushButtonAddTempl, SIGNAL(clicked()), this, SLOT(SendAddTemplate()));
    connect(ui->pushButtonClearTempl, SIGNAL(clicked()), this, SLOT(SendClearTemplate()));
     connect(ui->pushButtonDeleteTempl, SIGNAL(clicked()), this, SLOT(SendDeleteTemplate()));
     connect(ui->pushButtonGetAllTempl, SIGNAL(clicked()), this, SLOT(SendGetAllTemplate()));
    connect(ui->pushButtonGetSelectedTempl, SIGNAL(clicked()), this, SLOT(SendGetSelectedTemplate()));

}

void MainWindow::Connect()
{

       m_client->connectToHost(QHostAddress("127.0.0.1"), 6665);

}
void MainWindow::SendMsg()
{
    if (m_client->state() == QAbstractSocket::ConnectedState)
    {
     QString text = ui->lineEditSend->text();
     qDebug() << text;
     m_client->write( text.toStdString().c_str());
    }

}

void MainWindow::SendInitScanner()
{
    if (m_client->state() == QAbstractSocket::ConnectedState)
    {
        int nCount = 0;
        char buff[MAX_MSG_SIZE];
        memset(buff, 0, sizeof(buff));
        buff[nCount++] = START_FLAG;
        buff[nCount++] = INIT_SCANNER_CMD;
        buff[nCount++] = MID_FLAG;
        buff[nCount++] = END_FLAG;

        QString str = buff;
        qDebug() << str;
       m_client->write( (const char*)buff, nCount);
       ui->lineEditSend->setText( QString(printMessage(std::string(buff)).c_str()));
    }

}

void MainWindow::SendTriggerPicture()
{
    if (m_client->state() == QAbstractSocket::ConnectedState)
    {
        int nCount = 0;
        char buff[MAX_MSG_SIZE];
        memset(buff, 0, sizeof(buff));
        buff[nCount++] = START_FLAG;
        buff[nCount++] = TRIGGER_CMD;
        buff[nCount++] = MID_FLAG;
        buff[nCount++] = END_FLAG;

        QString str = buff;
        qDebug() << str;
       m_client->write( (const char*)buff, nCount);
       ui->lineEditSend->setText( QString(printMessage(std::string(buff)).c_str()));
    }

}
void MainWindow::SendCaptureTarget()
{
    if (m_client->state() == QAbstractSocket::ConnectedState)
    {
        int nCount = 0;
        char buff[MAX_MSG_SIZE];
        memset(buff, 0, sizeof(buff));
        buff[nCount++] = START_FLAG;
        buff[nCount++] = RECOGNITION_CMD;
        buff[nCount++] = MID_FLAG;

        std::string substr;
        {
            float xstart = 0;
            float ystart = 0;
            float xend = 4000;
            float yend = 4000;
            char subbuff[MAX_MSG_SIZE];
            memset(subbuff, 0, sizeof(subbuff));
            snprintf(subbuff, sizeof(subbuff),
                  "%.4f,%.4f,%.4f,%.4f",
                  xstart,xend,ystart,yend);
            substr = subbuff;
        }

        memcpy(buff + nCount, substr.c_str(), substr.size());
        nCount +=  substr.size();

        buff[nCount++] = END_FLAG;

        QString str = buff;
        qDebug() << str;
       m_client->write((const char*)buff, nCount);

        ui->lineEditSend->setText( QString(printMessage(std::string(buff)).c_str()));
    }
}

void MainWindow::SendCaptureTargetError()
{
    if (m_client->state() == QAbstractSocket::ConnectedState)
    {
        int nCount = 0;
        char buff[MAX_MSG_SIZE];
        memset(buff, 0, sizeof(buff));
        buff[nCount++] = START_FLAG;
        buff[nCount++] = RECOGNITION_CMD;
        buff[nCount++] = MID_FLAG;
        buff[nCount++] = END_FLAG;

        QString str = buff;
        qDebug() << str;
       m_client->write((const char*)buff, nCount);

        ui->lineEditSend->setText( QString(printMessage(std::string(buff)).c_str()));
         ui->lineEditRecv->setText("");
    }
}
void MainWindow::SendCaptureTargetResponseSuccess()
{
    if (m_client->state() == QAbstractSocket::ConnectedState)
    {
        int nCount = 0;
        char buff[MAX_MSG_SIZE];
        memset(buff, 0, sizeof(buff));
        buff[nCount++] = START_FLAG;
        buff[nCount++] = ROBOT_GRAB_RESPONSE_SUCCESS;
        buff[nCount++] = MID_FLAG;
        buff[nCount++] = END_FLAG;

        QString str = buff;
        qDebug() << str;
       m_client->write((const char*)buff, nCount);

        ui->lineEditSend->setText( QString(printMessage(std::string(buff)).c_str()));
         ui->lineEditRecv->setText("");
    }
}
void MainWindow::SendCaptureTargetResponseFail()
{
    if (m_client->state() == QAbstractSocket::ConnectedState)
    {
        int nCount = 0;
        char buff[MAX_MSG_SIZE];
        memset(buff, 0, sizeof(buff));
        buff[nCount++] = START_FLAG;
        buff[nCount++] = ROBOT_GRAB_RESPONSE_FAIL;
        buff[nCount++] = MID_FLAG;
        buff[nCount++] = END_FLAG;

        QString str = buff;
        qDebug() << str;
        m_client->write((const char*)buff, nCount);

        ui->lineEditSend->setText( QString(printMessage(std::string(buff)).c_str()));
        ui->lineEditRecv->setText("");
    }
}

void MainWindow::SendGetAllTemplate()
{
    if (m_client->state() == QAbstractSocket::ConnectedState)
    {
        int nCount = 0;
        char buff[MAX_MSG_SIZE];
        memset(buff, 0, sizeof(buff));
        buff[nCount++] = START_FLAG;
        buff[nCount++] = GET_ALL_TEMPL_CMD;
        buff[nCount++] = MID_FLAG;
        buff[nCount++] = END_FLAG;

        QString str = buff;
        qDebug() << str;
       m_client->write((const char*)buff, nCount);

        ui->lineEditSend->setText( QString(printMessage(std::string(buff)).c_str()));
         ui->lineEditRecv->setText("");
    }
}

void MainWindow::SendGetSelectedTemplate()
{
    if (m_client->state() == QAbstractSocket::ConnectedState)
    {
        int nCount = 0;
        char buff[MAX_MSG_SIZE];
        memset(buff, 0, sizeof(buff));
        buff[nCount++] = START_FLAG;
        buff[nCount++] = GET_SELECTED_TEMPL_CMD;
        buff[nCount++] = MID_FLAG;
        buff[nCount++] = END_FLAG;

        QString str = buff;
        qDebug() << str;
       m_client->write((const char*)buff, nCount);

        ui->lineEditSend->setText( QString(printMessage(std::string(buff)).c_str()));
         ui->lineEditRecv->setText("");
    }
}

void MainWindow::SendClearTemplate()
{
    if (m_client->state() == QAbstractSocket::ConnectedState)
    {
        int nCount = 0;
        char buff[MAX_MSG_SIZE];
        memset(buff, 0, sizeof(buff));
        buff[nCount++] = START_FLAG;
        buff[nCount++] = CLEAR_ALL_TEMPL_CMD;
        buff[nCount++] = MID_FLAG;
        buff[nCount++] = END_FLAG;

        QString str = buff;
        qDebug() << str;
       m_client->write((const char*)buff, nCount);

        ui->lineEditSend->setText( QString(printMessage(std::string(buff)).c_str()));
         ui->lineEditRecv->setText("");
    }
}

void MainWindow::SendAddTemplate()
{
    if (m_client->state() == QAbstractSocket::ConnectedState)
    {
        QString output;
        GetUserInputLineEdit(0,"AddTemplate","AddTemplate","H",output);
          std::string stdmsg = output.toStdString();
        int nCount = 0;
        char buff[MAX_MSG_SIZE];
        memset(buff, 0, sizeof(buff));
        buff[nCount++] = START_FLAG;
        buff[nCount++] = ADD_TEMPL_CMD;
        buff[nCount++] = MID_FLAG;
        if (!stdmsg.empty())
        {
            memcpy(buff + nCount, stdmsg.c_str(), stdmsg.size());
            nCount +=  stdmsg.size();
        }
        buff[nCount++] = END_FLAG;

        QString str = buff;
        qDebug() << str;
       m_client->write((const char*)buff, nCount);

        ui->lineEditSend->setText( QString(printMessage(std::string(buff)).c_str()));
         ui->lineEditRecv->setText("");
    }
}

void MainWindow::SendDeleteTemplate()
{
    if (m_client->state() == QAbstractSocket::ConnectedState)
    {
        QString output;
        GetUserInputLineEdit(0,"DeleteTemplate","DeleteTemplate","H",output);
        std::string stdmsg = output.toStdString();

        int nCount = 0;
        char buff[MAX_MSG_SIZE];
        memset(buff, 0, sizeof(buff));
        buff[nCount++] = START_FLAG;
        buff[nCount++] = DELETE_TEMPL_CMD;
        buff[nCount++] = MID_FLAG;
        if (!stdmsg.empty())
        {
            memcpy(buff + nCount, stdmsg.c_str(), stdmsg.size());
            nCount +=  stdmsg.size();
        }
        buff[nCount++] = END_FLAG;

        QString str = buff;
        qDebug() << str;
        m_client->write((const char*)buff, nCount);

        ui->lineEditSend->setText( QString(printMessage(std::string(buff)).c_str()));
         ui->lineEditRecv->setText("");
    }
}

void MainWindow::readClient()
{
    qDebug() << "readClient";
   char buf[MAX_MSG_SIZE] = "";
   m_client->read(buf,MAX_MSG_SIZE);

    ui->lineEditRecv->setText( QString(printMessage(std::string(buf)).c_str()));
    ui->lineEditSend->setText("");
}

MainWindow::~MainWindow()
{
    delete ui;
}
