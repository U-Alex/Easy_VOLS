#include "objpwcont.h"

ObjPwcont::ObjPwcont(int p_type, QStringList params) : QGraphicsObject()
{
    this->setFlags(ItemIsSelectable);
    type = p_type;
    p_rect = QRectF(-7, -7, 14, 14);
    p_color = QColor(params.at(0));
}

QRectF ObjPwcont::boundingRect() const
{
    return p_rect;
}

void ObjPwcont::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(p_color);
    if (isSelected()) painter->setBrush(QColor(Qt::magenta));
    if (type == 1) {
        painter->drawRect(p_rect);
        painter->drawLine(-7,-7,-4,-4);
        painter->drawLine( 7,-7, 4,-4);
        painter->drawLine(-7, 7,-4, 4);
        painter->drawLine( 7, 7, 4, 4);
    }
    else if (type == 2) {
        painter->drawEllipse(p_rect);
        painter->drawRect(QRectF(-5, -5, 10, 10));
    }
}

