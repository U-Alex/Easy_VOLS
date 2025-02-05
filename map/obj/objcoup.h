#ifndef OBJCOUP_H
#define OBJCOUP_H

#include <QGraphicsObject>
#include <QPainter>


class ObjCoup : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ObjCoup(QMap<QString, QString> params);
    explicit ObjCoup();                     //
//    ~ObjCoup();

    QRectF boundingRect() const;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


private:
//    QString     name;
    QRectF      p_rect;

};

#endif // OBJCOUP_H
