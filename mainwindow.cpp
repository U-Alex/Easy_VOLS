#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

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
    mapManager = new MapManager(this);
    ui->map_manager_Layout->addWidget(mapManager);

    qDebug() << "start";

}
