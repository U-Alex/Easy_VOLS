#include "objpolyline.h"

ObjPolyline::ObjPolyline(QObject *parent) : QObject(parent)
{
    this->setAcceptHoverEvents(true);
    this->setFlags(ItemIsSelectable|ItemSendsGeometryChanges);

}

void ObjPolyline::setPath(const QPainterPath &path)
{
    QGraphicsPathItem::setPath(path);
}

void ObjPolyline::setMove(bool ok)
{
    move_mode = ok;
}

void ObjPolyline::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (! move_mode) return;
    QPointF clickPos = event->pos();
    QLineF checkLineFirst(clickPos.x() - 5, clickPos.y() - 5, clickPos.x() + 5, clickPos.y() + 5);
    QLineF checkLineSecond(clickPos.x() + 5, clickPos.y() - 5, clickPos.x() - 5, clickPos.y() + 5);
    QPainterPath oldPath = path();
    QPainterPath newPath;
    for(int i = 0; i < oldPath.elementCount(); i++){
        QLineF checkableLine(oldPath.elementAt(i), oldPath.elementAt(i+1));
        if(checkableLine.intersects(checkLineFirst, 0) == 1 || checkableLine.intersects(checkLineSecond, 0) == 1) {
            if(i == 0){
                newPath.moveTo(oldPath.elementAt(i));
                newPath.lineTo(clickPos);
            } else {
                newPath.lineTo(oldPath.elementAt(i));
                newPath.lineTo(clickPos);
            }
        } else {
            if(i == 0){
                newPath.moveTo(oldPath.elementAt(i));
            } else {
                newPath.lineTo(oldPath.elementAt(i));
            }
        }
        if(i == (oldPath.elementCount() - 2)) {
            newPath.lineTo(oldPath.elementAt(i + 1));
            i++;
        }
    }
    setPath(newPath);
    updateDots();
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void ObjPolyline::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if (! listDotes.isEmpty()) {
        foreach (ObjDotSignal *dot, listDotes) {
            dot->deleteLater();
        }
        listDotes.clear();
    }
    QGraphicsItem::hoverLeaveEvent(event);
}

void ObjPolyline::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverMoveEvent(event);
}

void ObjPolyline::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (! move_mode || ! isSelected()) return;                      //режим edit и выбранная линия
    QPainterPath linePath = path();
    for (int i = 0; i < linePath.elementCount(); i++) {
        QPointF point = linePath.elementAt(i);
        ObjDotSignal *dot = new ObjDotSignal(point, this);
        if (i != 0 && i != linePath. elementCount() - 1) {          //крайние точки не двигать
            connect(dot, &ObjDotSignal::signalMove, this, &ObjPolyline::slotMove);
        }
        connect(dot, &ObjDotSignal::signalMouseRelease, this, &ObjPolyline::checkForDeletePoints);
        dot->setDotFlags(ObjDotSignal::Movable);
        listDotes.append(dot);
    }
    QGraphicsItem::hoverEnterEvent(event);
}

void ObjPolyline::slotMove(QGraphicsItem *signalOwner, qreal dx, qreal dy)
{
    QPainterPath linePath = path();
    for (int i = 0; i < linePath.elementCount(); i++) {
        if (listDotes.at(i) == signalOwner) {
            QPointF pathPoint = linePath.elementAt(i);
            linePath.setElementPositionAt(i, pathPoint.x() + dx, pathPoint.y() + dy);
            m_pointForCheck = i;
        }
    }
    setPath(linePath);
}

void ObjPolyline::checkForDeletePoints()
{
//qWarning() << "checkForDeletePoints" << m_pointForCheck;
    if(m_pointForCheck != -1){
        QPainterPath linePath = path();

        QPointF pathPoint = linePath.elementAt(m_pointForCheck);
        if (m_pointForCheck > 0) {
            QLineF lineToNear(linePath.elementAt(m_pointForCheck-1),pathPoint);
            if (lineToNear.length() < 6.0) {
                QPainterPath newPath;
                newPath.moveTo(linePath.elementAt(0));
                for (int i = 1; i < linePath.elementCount(); i++) {
                    if (i != m_pointForCheck)
                        newPath.lineTo(linePath.elementAt(i));
                }
                setPath(newPath);
            }
        }
        if (m_pointForCheck < linePath.elementCount() - 1) {
            QLineF lineToNear(linePath.elementAt(m_pointForCheck+1),pathPoint);
            if(lineToNear.length() < 6.0) {
                QPainterPath newPath;
                newPath.moveTo(linePath.elementAt(0));
                for(int i = 1; i < linePath.elementCount(); i++){
                    if(i != m_pointForCheck)
                        newPath.lineTo(linePath.elementAt(i));
                }
                setPath(newPath);
            }
        }
        updateDots();
        m_pointForCheck = -1;
    }
}

void ObjPolyline::updateDots()
{
    if (! listDotes.isEmpty()){
        foreach (ObjDotSignal *dot, listDotes) {
            dot->deleteLater();
        }
        listDotes.clear();
    }
    QPainterPath linePath = path();
    for (int i = 0; i < linePath.elementCount(); i++) {
        QPointF point = linePath.elementAt(i);
        ObjDotSignal *dot = new ObjDotSignal(point, this);
        if (i != 0 && i != linePath. elementCount() - 1) {          //крайние точки не двигать
            connect(dot, &ObjDotSignal::signalMove, this, &ObjPolyline::slotMove);
        }
        connect(dot, &ObjDotSignal::signalMouseRelease, this, &ObjPolyline::checkForDeletePoints);
        dot->setDotFlags(ObjDotSignal::Movable);
        listDotes.append(dot);
    }
    emit sigClick(this);
}
