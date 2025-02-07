#include "map/obj/objfab.h"
#include "map/obj/obj.h"
#include "map/obj/objcoup.h"
#include "map/obj/objlocker.h".h"
//#include <QJsonValue>

ObjFab::ObjFab() {}

QList<QGraphicsObject*> ObjFab::createCoup(QJsonDocument json)
{
    ObjCoup *coup;
    QList<QGraphicsObject*> obj_list{};
    QJsonValue ob;
    int type;
    QColor color = Qt::green;   //

    for (auto i = 0; !json[i].isUndefined() ; ++i) {
        ob = json[i];
        if (ob["installed"].toBool()) {
            type = ob["parr_type"].toInt(0);
//            color = (owner.contains(conf->your_owner)) ? conf->color_coup.at(type+1) : conf->color_coup.at(4);
        }
        else {
            type = -1;
//            color = conf->color_coup.at(5);
        }
        coup = new ObjCoup(type, color);
        coup->setData((int)Idx::label, "coup");
        coup->setData((int)Idx::o_id, ob["id"].toInt(0));
//        coup->setData((int)Idx::parr_type, type);
//        coup->setData((int)Idx::o_name, ob["name"]);
        quint16 xx = ob["coord_x"].toInt(); if (xx < 20) xx = 20;
        quint16 yy = ob["coord_y"].toInt(); if (yy < 20) yy = 20;
        coup->setPos(QPoint(xx, yy));
        coup->setZValue(10);
        obj_list.append(static_cast<QGraphicsObject*>(coup));
    }
    return obj_list;
}

QList<QGraphicsObject*> ObjFab::createLocker(QJsonDocument json)
{
    ObjLocker *lo;
    QList<QGraphicsObject*> obj_list{};
    QJsonValue ob;
    for (auto i = 0; !json[i].isUndefined() ; ++i) {
        ob = json[i];

        lo = new ObjLocker(1,
                           {ob["name"].toString(),
                            "в работе",
                            "yellow",
                            "blue",
                            ob["co"].toString(),
                            ob["object_owner"].toString()
                           });
        lo->setData((int)Idx::label, "locker");
        lo->setData((int)Idx::o_id, ob["id"].toInt(0));
        quint16 xx = ob["coord_x"].toInt(); if (xx < 20) xx = 20;
        quint16 yy = ob["coord_y"].toInt(); if (yy < 20) yy = 20;
        lo->setPos(QPoint(xx, yy));
        lo->setZValue(9);
        obj_list.append(static_cast<QGraphicsObject*>(lo));
    }
    return obj_list;
}
