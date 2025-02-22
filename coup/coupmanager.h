#ifndef COUPMANAGER_H
#define COUPMANAGER_H

#include <QWidget>
//#include <QJsonDocument>

#include "config.h"
#include "b_logic/usersession.h"
#include "coup/couppaint.h"
#include "coup/couppaintext.h"

namespace Ui { class CoupManager; }

class CoupManager : public QWidget
{
    Q_OBJECT
public:
    explicit CoupManager(Config *ref_conf, UserSession *us, QWidget *parent = nullptr);
    ~CoupManager();


public slots:
    void slotNextCoup(uint, QPoint);
    void nextCoup(uint);
    void slotNextCoupExt(QStringList, QStringList);
    void slotShowHopExt(uint);
//    void slotShowHop(uint, QJsonDocument);

signals:
    void sigToMapCoup(QPoint);

private:
    Ui::CoupManager *ui;
    Config          *conf = nullptr;
    UserSession     *userSession = nullptr;
    uint             coup_id = 0;

    CoupPaint       *coupPaint = nullptr;
    CoupPaintExt    *coupPaintExtL = nullptr;
    CoupPaintExt    *coupPaintExtR = nullptr;

    QStringList      cab_L, cab_R;

private slots:
    void on_pb_L_toggled(bool);
    void on_pb_R_toggled(bool);
    void fr_P_repaint(short);

    void on_spinBox_w_line_valueChanged(int);
    void on_pb_update_clicked();
    void on_pb_export_clicked();
    void on_pb_print_clicked();
};

#endif // COUPMANAGER_H
