#include "map/obj/objcoup.h"

#include <QDebug>

ObjCoup::ObjCoup(QMap<QString, QString> params) : QGraphicsObject()
{
qDebug() << "constr ObjCoup: " << params;
    this->setFlags(ItemIsSelectable);
    this->setZValue(10);
//    this->setData()
    p_rect = QRectF(-10, -10, 20, 20);
}

ObjCoup::~ObjCoup() {}

QRectF ObjCoup::boundingRect() const
{
//    return p_rect;
    return QRectF(-10, -10, 20, 20);
}

void ObjCoup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

//    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//    //QColor fillColor = (option->state & QStyle::State_Selected) ? p_color.dark(150) : p_color;
//    painter->setBrush(p_color);
//    if (isSelected()) painter->setBrush(QColor(Qt::magenta));
//    //if (flags() & ItemIsMovable) painter->setBrush(Qt::red);
//    if (data(7) == 2) painter->setBrush(Qt::red);                                      //ObjVisMode
//    if (data(7) == 0) painter->setBrush(Qt::gray);                                     //ObjVisMode
    painter->drawEllipse(p_rect);
}
