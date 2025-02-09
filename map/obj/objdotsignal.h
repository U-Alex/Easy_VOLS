#ifndef OBJDOTSIGNAL_H
#define OBJDOTSIGNAL_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
//class QGraphicsSceneHoverEventPrivate;
//class QGraphicsSceneMouseEvent;

class ObjDotSignal : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
//    Q_PROPERTY(QPointF previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)
public:
    explicit ObjDotSignal(QGraphicsItem *parentItem = 0, QObject *parent = 0);
    explicit ObjDotSignal(QPointF pos, QGraphicsItem *parentItem = 0, QObject *parent = 0);

    enum Flags { Movable = 0x01 };

    QPointF previousPosition() const;
    void setPreviousPosition(const QPointF previousPosition);
    void setDotFlags(uint flags);

signals:
    void previousPositionChanged();
    void signalMouseRelease();
    void signalMove(QGraphicsItem *signalOwner, qreal dx, qreal dy);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    unsigned int m_flags;
    QPointF m_previousPosition;

};

#endif // OBJDOTSIGNAL_H
