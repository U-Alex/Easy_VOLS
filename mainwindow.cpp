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
    ui->but_logout->hide();
    slotAuthResult(true);   //
}

MainWindow::~MainWindow()
{
//    if (mapManager != nullptr) mapManager->deleteLater();
//    userSession->deleteLater();
    delete ui;
}

void MainWindow::slotAuthResult(bool ok)
{
//    qDebug() << "authResult" << ok;
    if (ok) {
        ui->but_login->hide();
        ui->lineEdit_login->hide();
        ui->lineEdit_password->hide();
        ui->but_logout->setVisible(true);
        ui->label_user->setText("TODO user");
        if (mapManager == nullptr) {
            mapManager = new MapManager(conf, userSession, this);
            mapManager->setAttribute(Qt::WA_DeleteOnClose, 1);
            mapManager->setWindowFlags(Qt::Window);
            connect(mapManager, &MapManager::destroyed, this, &MainWindow::on_but_logout_clicked);
            mapManager->show();
        }
    }
    else {
        ui->label_user->setText("not auth");
    }
}

void MainWindow::on_but_login_clicked()
{
    userSession->auth(ui->lineEdit_login->text(), ui->lineEdit_password->text());
}

void MainWindow::on_but_logout_clicked()
{
    ui->but_logout->hide();
    ui->but_login->setVisible(true);
    ui->lineEdit_login->setVisible(true);
    ui->lineEdit_password->setVisible(true);
    ui->label_user->setText("авторизация");
    if (mapManager != nullptr) {
        mapManager->deleteLater();
        mapManager = nullptr;
    }
}

