#include "objfab.h"
#include "map/obj/objcoup.h"
#include <QJsonValue>

ObjFab::ObjFab() {}

QList<QGraphicsObject*> ObjFab::createCoup(QJsonDocument json)
{
    ObjCoup *coup;
    QList<QGraphicsObject*> obj_list{};
    QJsonValue ob;

    for (auto i = 0; !json[i].isUndefined() ; ++i) {
        ob = json[i];
        coup = new ObjCoup();
        coup->setData((int)Idx::o_id, ob["id"]);
        coup->setData((int)Idx::o_name, ob["name"]);
        quint16 xx = ob["coord_x"].toInt(); if (xx < 20) xx = 20;
        quint16 yy = ob["coord_y"].toInt(); if (yy < 20) yy = 20;
        coup->setPos(QPoint(xx, yy));
        obj_list.append(static_cast<QGraphicsObject*>(coup));
    }
    return obj_list;
}
