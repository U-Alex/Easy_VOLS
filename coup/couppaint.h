#ifndef COUPPAINT_H
#define COUPPAINT_H

#include <QFrame>
#include <QJsonDocument>
#include <QJsonObject>

#include "config.h"

namespace Ui { class CoupPaint; }

class CoupPaint : public QFrame
{
    Q_OBJECT
public:
    explicit CoupPaint(Config *ref_conf, uint c_id, QWidget *parent = nullptr);
    ~CoupPaint();

public slots:
    void slotCoupPaint(uint, QJsonDocument);

signals:
//    void fr_repaint(short);
    void sigNextCoup(int, QPoint);
    void sigNextCoupExt(QStringList, QStringList);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::CoupPaint *ui;
    Config        *conf = nullptr;
    uint           coup_id = 0;

    QList<QJsonObject>              coup_ports;
    QList<QJsonObject>              cab_links;
    QList<QJsonObject>  cab_slot_L, cab_slot_R;

    QMap<int, QPair<int, QString>>  cross_p;
    bool                            f_color = true;

    QMap<int, QPoint>   ports;
    QVector<QPolygon>   links;
    QVector<QPen>       links_pen;
    QList<QPolygon>     links_sel;
    QVector<QRect>      circle0, circle1;
    QRect               back_list;

private slots:
    void printHead(uint, QJsonValue, QJsonValue);
    void prepareCab();
    void postCabList();
    void createBut();
    void createLinks();

    void but_ext_coup_clicked();
    void but_cab_clicked();
    void but_fiber_clicked();
};

#endif // COUPPAINT_H
