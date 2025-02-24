#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "b_logic/logger.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Logger::setTextEdit(ui->textEdit_log);
    conf = new Config();
    userSession = new UserSession(conf, this);
    QObject::connect(userSession, &UserSession::sigAuthResult, this, &MainWindow::slotAuthResult);
    ui->but_logout->hide();
//    slotAuthResult(true, "");   //
}

MainWindow::~MainWindow()
{
    if (mapManager != nullptr) mapManager->deleteLater();
    userSession->deleteLater();
    delete ui;
}

void MainWindow::slotAuthResult(bool ok, QString username)
{
    Logger::sendLog({"log in:", username});
    if (ok) {
        ui->but_login->hide();
        ui->lineEdit_login->hide();
        ui->lineEdit_password->hide();
        ui->but_logout->setVisible(true);
        ui->label_user->setText(username);
        if (mapManager == nullptr) {
            mapManager = new MapManager(conf, userSession, this);
            mapManager->setAttribute(Qt::WA_DeleteOnClose, true);
            mapManager->setWindowFlags(Qt::Window);
            QObject::connect(mapManager, &MapManager::destroyed, this, &MainWindow::on_but_logout_clicked);
            mapManager->show();
            mapManager->start();
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
    if (mapManager != nullptr) {
        mapManager->deleteLater();
        mapManager = nullptr;
        Logger::sendLog({"logout:", ui->label_user->text()});
        ui->label_user->setText("авторизация");
    }
}

