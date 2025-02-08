#ifndef OBJPWCONT_H
#define OBJPWCONT_H

#include <QGraphicsObject>
#include <QPainter>


class ObjPwcont : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ObjPwcont(int p_type, QStringList params);

    QRectF boundingRect() const;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QRectF      p_rect;
    QColor      p_color;
    int         type;

};

#endif // OBJPWCONT_H
