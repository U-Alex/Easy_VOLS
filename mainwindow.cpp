#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start()
{
    userSession = new UserSession(this);
    QObject::connect(userSession, &UserSession::authResult, this, &MainWindow::authResult);
    //ui form
    userSession->auth("admin", "admin");
//    userSession->auth("uaa", "celfHM123");

}

void MainWindow::authResult(bool ok)
{
//    qDebug() << "authResult" << ok;

    mapManager = new MapManager(this, userSession);
//    QObject::connect(userSession, &UserSession::objResult, mapManager, &MapManager::objRecieve);
    ui->map_manager_Layout->addWidget(mapManager);
}
