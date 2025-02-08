#ifndef OBJPOLYLINE_H
#define OBJPOLYLINE_H

#include <QObject>
#include <QGraphicsPathItem>


class ObjPolyline : public QObject, public QGraphicsPathItem
{
    Q_OBJECT
public:
    explicit ObjPolyline(QObject *parent = nullptr);

    void setPath(const QPainterPath &path);

signals:
    void sigClick(QGraphicsItem *);






};

#endif // OBJPOLYLINE_H
