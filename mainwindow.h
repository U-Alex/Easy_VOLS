#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

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

    void start();

public slots:
    void authResult(bool);

private:
    Ui::MainWindow *ui;

    MapManager     *mapManager;
    UserSession    *userSession;

};

#endif // MAINWINDOW_H
