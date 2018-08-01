#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "communwin.h"

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
    void OnCatchMessage(QString str);
    void SendMsg();
private:
    Ui::MainWindow *ui;
    CommunWin *m_pCommunWin;
};

#endif // MAINWINDOW_H
