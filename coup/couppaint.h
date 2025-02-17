#ifndef COUPPAINT_H
#define COUPPAINT_H

#include <QFrame>
#include <QJsonDocument>
#include <QJsonObject>

#include "config.h"
//#include "b_logic/usersession.h"

namespace Ui { class CoupPaint; }

class CoupPaint : public QFrame
{
    Q_OBJECT
public:
    explicit CoupPaint(Config *ref_conf, /*UserSession *us, */QWidget *parent = nullptr);
    ~CoupPaint();

public slots:
    void slotCoupPaint(uint, QJsonDocument);

signals:
//    void fr_repaint(short);
    void nextCoup(int, QPoint);

private:
    Ui::CoupPaint *ui;
    Config        *conf = nullptr;
//    UserSession   *userSession = nullptr;

    QList<QJsonObject> coup_ports;
    QList<QJsonObject> cab_links;
    QList<QJsonObject> cab_slot_L, cab_slot_R;

    QMap<int,QPoint>   ports;

private slots:
    void printHead(uint, QJsonValue, QJsonValue);
    void prepareCab();
    void createBut();
    void createLinks();

    void but_ext_coup_clicked();
};

#endif // COUPPAINT_H
