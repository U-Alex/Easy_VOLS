#include "objlocker.h"
#include <QRegularExpression>

ObjLocker::ObjLocker(int l_type, QStringList params) : QGraphicsObject()
{
    this->setFlags(ItemIsSelectable);
    type = l_type;
    if (type == 0) p_rect = QRectF(-5, -15, 55, 30);
    if (type == 1) p_rect = QRectF(-40, -40, 80, 80);
    if (type == 2) p_rect = QRectF(-5, -10, 55, 20);
    name = params.at(0);
    status = params.at(1);
    status_color = params.at(2);
    co_color = params.at(3);
    co = params.at(4);
    owner = params.at(5);
    QList<QString> n1 = name.split("-");        //TODO  split -> re
    QRegularExpression re("\\d*");
    if (type == 0 && n1.length() > 2)
        if (re.match(n1.at(2)).hasMatch())
            name_lite = n1.at(2);
}

QRectF ObjLocker::boundingRect() const
{
    return p_rect;
}

void ObjLocker::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(co_color);
    if (isSelected()) painter->setBrush(QColor(Qt::magenta));
//    if (data(7) == 2) painter->setBrush(Qt::red);                                  //ObjVisMode
//    if (data(7) == 0) painter->setBrush(Qt::gray);                                 //ObjVisMode
    QPainterPath path;

    if (type == 1) {
        path.moveTo(0, -11);
        path.arcTo(QRectF(-11,-11,22,22), 90, 360);
        path.moveTo(0, -40);
        path.lineTo(35, 20);
        path.lineTo(-35, 20);
        path.lineTo(0, -40);

        painter->drawPath(path);
    }

    if (type == 0) {
        path.moveTo(0, -10);
        path.cubicTo(5, -14,  5, -14,  11, -15);
        path.lineTo(50, -15);
        path.lineTo(50, 15);
        path.lineTo(11, 15);
        path.cubicTo(5, 14,  5, 14,  0, 10);
        path.arcTo(QRectF(-10,-10,20,20), 270, 180);

        painter->drawPath(path);

        painter->setBrush(status_color);
        painter->drawRect(QRectF(27, 9, 23, 6));
    }

    if (type == 2) {
        path.moveTo(0, -10);
        path.lineTo(50, -10);
        path.lineTo(50, 10);
        path.lineTo(0, 10);
        path.arcTo(QRectF(-10,-10,20,20), 270, 180);

        painter->drawPath(path);

        painter->setBrush(status_color);
        painter->drawRect(QRectF(44, 4, 6, 6));
    }

    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());

    if (lod <= 1) {
        QFont font("Sans", 140);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        painter->scale(0.1, 0.1);
        painter->drawText(140, 60, name_lite);
        painter->restore();
    }
    else if (lod < 3 && lod > 1) {
        QFont font("Sans", 34);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        painter->scale(0.1, 0.1);
        if (type == 0)
            painter->drawText(110, -100, 370, 140, Qt::AlignRight | Qt::TextWordWrap, name);
        else if (type == 1)
            painter->drawText(-200, 90, 400, 140, Qt::AlignCenter | Qt::TextWordWrap, name);
        else
            painter->drawText(110, -95, 370, 140, Qt::AlignRight | Qt::TextWordWrap, name);

        painter->restore();
    }
    else if (lod >= 3) {
        QFont font("Times", 30);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        painter->scale(0.1, 0.1);

        if (type == 0) {
            painter->drawText(110, -80, 370, 140, Qt::AlignRight | Qt::TextWordWrap, name);
            painter->drawText(290, 130, status);
            painter->drawText(100, 70, owner);
            painter->drawText(100, -100, co);
        }
        else if (type == 1) {
            painter->drawText(-300, 110, 600, 100, Qt::AlignCenter | Qt::TextWordWrap, name);
        }
        else  {
            painter->drawText(110, -75, 360, 140, Qt::AlignRight | Qt::TextWordWrap, name);
            painter->drawText(100, 80, owner);
        }
        painter->restore();
    }

}
