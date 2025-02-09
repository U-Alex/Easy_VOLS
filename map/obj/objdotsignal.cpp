#include "objdotsignal.h"

#include <QBrush>

ObjDotSignal::ObjDotSignal(QGraphicsItem *parentItem, QObject *parent) :
    QObject(parent)
{
    this->setParentItem(parentItem);
    this->setAcceptHoverEvents(true);
    this->setBrush(QBrush(Qt::black));
    this->setRect(-4,-4,8,8);
    setDotFlags(0);
}

ObjDotSignal::ObjDotSignal(QPointF pos, QGraphicsItem *parentItem, QObject *parent) :
    ObjDotSignal(parentItem, parent)
{
//    setParentItem(parentItem);
//    setAcceptHoverEvents(true);
//    setBrush(QBrush(Qt::black));
//    setRect(-4,-4,8,8);
    this->setPos(pos);
    this->setPreviousPosition(pos);
//    setDotFlags(0);
}

//ObjDotSignal::ObjDotSignal(QPointF pos, QGraphicsItem *parentItem, QObject *parent) :
//    QObject(parent)
//{
//    setParentItem(parentItem);
//    setAcceptHoverEvents(true);
//    setBrush(QBrush(Qt::black));
//    setRect(-4,-4,8,8);
////    setPos(pos);
////    setPreviousPosition(pos);
//    setDotFlags(0);
//}

QPointF ObjDotSignal::previousPosition() const
{
    return m_previousPosition;
}

void ObjDotSignal::setDotFlags(uint flags)
{
    m_flags = flags;
}

void ObjDotSignal::setPreviousPosition(const QPointF previousPosition)
{
    if (m_previousPosition == previousPosition) return;
    m_previousPosition = previousPosition;
    emit previousPositionChanged();
}

void ObjDotSignal::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_flags & Movable){
        if (event->scenePos().x() > 20 && event->scenePos().y() > 20) {//out of size
            auto dx = event->scenePos().x() - m_previousPosition.x();
            auto dy = event->scenePos().y() - m_previousPosition.y();
            moveBy(dx,dy);
            setPreviousPosition(event->scenePos());
            emit signalMove(this, dx, dy);
        }
    } else {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void ObjDotSignal::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_flags & Movable)
        setPreviousPosition(event->scenePos());
    else
        QGraphicsItem::mousePressEvent(event);
}

void ObjDotSignal::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit signalMouseRelease();
    QGraphicsItem::mouseReleaseEvent(event);
}

void ObjDotSignal::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    setBrush(QBrush(Qt::red));
}

void ObjDotSignal::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    setBrush(QBrush(Qt::black));
}
