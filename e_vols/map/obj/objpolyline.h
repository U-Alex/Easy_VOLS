#ifndef OBJPOLYLINE_H
#define OBJPOLYLINE_H

#include <QObject>
#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>

#include "map/obj/objdotsignal.h"

class ObjPolyline : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    explicit ObjPolyline(QObject *parent = nullptr);

    void setPath(const QPainterPath &path);

public slots:
    void setMove(bool);

signals:
    void sigClick(QGraphicsItem *);

protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
//    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

private:
    QList<ObjDotSignal *>   listDotes;
    int                     m_pointForCheck = -1;
    bool                    move_mode = false;

private slots:
    void slotMove(QGraphicsItem *signalOwner, qreal dx, qreal dy);
    void updateDots();
    void deleteDots();
//    void checkForDeletePoints();

};

#endif // OBJPOLYLINE_H
