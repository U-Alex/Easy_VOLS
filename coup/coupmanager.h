#ifndef COUPMANAGER_H
#define COUPMANAGER_H

#include <QWidget>

#include "config.h"
#include "b_logic/usersession.h"
#include "coup/couppaint.h"

namespace Ui { class CoupManager; }

class CoupManager : public QWidget
{
    Q_OBJECT
public:
    explicit CoupManager(Config *ref_conf, UserSession *us, QWidget *parent = nullptr);
    ~CoupManager();


public slots:
    void nextCoup(uint, QPoint);

signals:
    void sigToMapCoup(/*uint, */QPoint);

private:
    Ui::CoupManager *ui;
    Config          *conf = nullptr;
    UserSession     *userSession = nullptr;

    CoupPaint       *coupPaint = nullptr;

    int              coup_id = 0;


private slots:
    void on_pb_L_toggled(bool checked);
    void on_pb_R_toggled(bool checked);
    void fr_P_repaint(short);


};

#endif // COUPMANAGER_H
