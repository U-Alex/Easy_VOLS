#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include "config.h"
#include "map/mapmanager.h"
#include "b_logic/usersession.h"
//#include "b_logic/logger.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slotAuthResult(bool, QString);

private:
    Ui::MainWindow *ui;
    Config         *conf = nullptr;
    MapManager     *mapManager = nullptr;
    UserSession    *userSession = nullptr;
//    Logger         *logger = nullptr;

private slots:
    void on_but_login_clicked();
    void on_but_logout_clicked();
};

#endif // MAINWINDOW_H
