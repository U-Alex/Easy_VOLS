#include "map/obj/objcoup.h"


ObjCoup::ObjCoup(int parr_type, QColor color) : QGraphicsObject()
{
    this->setFlags(ItemIsSelectable);
    if     (parr_type == -1) p_rect = QRectF(-6, -6, 12, 12);
    else if (parr_type == 0) p_rect = QRectF(-8, -8, 16, 16);
         else                p_rect = QRectF(-10, -10, 20, 20);
    p_color = color;
}

QRectF ObjCoup::boundingRect() const
{
    return p_rect;
}

void ObjCoup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(p_color);
    if (isSelected()) painter->setBrush(QColor(Qt::magenta));
//    if (data(7) == 2) painter->setBrush(Qt::red);                                      //ObjVisMode
//    if (data(7) == 0) painter->setBrush(Qt::gray);                                     //ObjVisMode

    painter->drawEllipse(p_rect);
}

