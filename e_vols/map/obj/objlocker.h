#ifndef OBJLOCKER_H
#define OBJLOCKER_H

#include <QGraphicsObject>
#include <QPainter>
#include <QStyleOptionGraphicsItem>


class ObjLocker : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ObjLocker(int l_type, QStringList params);

    QRectF boundingRect() const;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QRectF      p_rect;
    int         type;
    QString     name, name_lite;
    QString     status, co, owner;
    QColor      status_color, co_color;


};

#endif // OBJLOCKER_H
