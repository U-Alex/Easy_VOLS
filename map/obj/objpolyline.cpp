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





