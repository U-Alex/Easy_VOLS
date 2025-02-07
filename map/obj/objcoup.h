#ifndef OBJCOUP_H
#define OBJCOUP_H

#include <QGraphicsObject>
#include <QPainter>


class ObjCoup : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ObjCoup(int parr_type, QColor color = Qt::gray);

    QRectF boundingRect() const;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QRectF      p_rect;
    QColor      p_color;

};

#endif // OBJCOUP_H
