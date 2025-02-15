#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

#include "config.h"
#include "map/mapmanager.h"
#include "b_logic/usersession.h"

//QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
//QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void slotAuthResult(bool);

private slots:
    void on_but_login_clicked();

    void on_but_logout_clicked();

private:
    Ui::MainWindow *ui;

    Config         *conf = nullptr;
    MapManager     *mapManager = nullptr;
    UserSession    *userSession = nullptr;

};

#endif // MAINWINDOW_H
