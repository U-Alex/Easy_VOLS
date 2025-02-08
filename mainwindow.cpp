#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    conf = new Config();
    userSession = new UserSession(this);
    QObject::connect(userSession, &UserSession::sigAuthResult, this, &MainWindow::slotAuthResult);
    this->start();  //
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start()
{
    //ui form
    userSession->auth("admin", "admin");
//    userSession->auth("uaa", "celfHM123");

}

void MainWindow::slotAuthResult(bool ok)
{
//    qDebug() << "authResult" << ok;

    mapManager = new MapManager(this, conf, userSession);
    ui->map_manager_Layout->addWidget(mapManager);
}
