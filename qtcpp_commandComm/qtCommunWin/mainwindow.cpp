#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pCommunWin = new CommunWin();
    connect(m_pCommunWin,SIGNAL(MsgSignal(QString)),this, SLOT(OnCatchMessage(QString)));
    //connect(ui->pushButtonConnect, SIGNAL(clicked()), m_pCommunWin, SLOT(Start()));
    connect(ui->pushButtonSend, SIGNAL(clicked()), this, SLOT(SendMsg()));

    m_pCommunWin->Start();
}


void MainWindow::OnCatchMessage(QString str)
{
    qDebug() << "OnCatchMessage";
    ui->lineEditRecv->setText(str);
}

void MainWindow::SendMsg()
{
    qDebug() << "OnCatchMessage";
   QString msg =  ui->lineEditSend->text();
   m_pCommunWin->sendClient(msg);
}

MainWindow::~MainWindow()
{
    delete ui;
}
