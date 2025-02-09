#include "b_logic/objfab.h"
#include "map/obj/obj.h"
#include "map/obj/objcoup.h"
#include "map/obj/objlocker.h"
#include "map/obj/objpwcont.h"
#include "map/obj/objpolyline.h"


ObjFab::ObjFab(Config *ref_conf, MapScene *scene, QObject *parent) :
    QObject(parent),
    conf(ref_conf),
    scene(scene)
{}

void ObjFab::slotDataToObj(ObjType objType, uint id, QJsonDocument json)
{
    if (!id) {
        switch(objType) {
            case ObjType::o_pw_cont:
                this->createPwcont(json);
                break;
            case ObjType::o_locker:
                this->createLocker(json);
                break;
            case ObjType::o_coup:
                this->createCoup(json);
                break;
            case ObjType::o_polyline:
                this->createPolyline(json);
                break;
        }
    }
}

void ObjFab::createPwcont(QJsonDocument json)
{
    ObjPwcont *pw;
    QJsonValue ob;
    for (auto i = 0; !json[i].isUndefined() ; ++i) {
        ob = json[i];

        pw = new ObjPwcont(ob["obj_type"].toInt(0),
                           {"#99CCCC",});
        pw->setData((int)Idx::label, "pwcont");
        pw->setData((int)Idx::o_id, ob["id"].toInt(0));
        pw->setData((int)Idx::o_name, ob["name"]);
        quint16 xx = ob["coord_x"].toInt(); if (xx < 20) xx = 20;
        quint16 yy = ob["coord_y"].toInt(); if (yy < 20) yy = 20;
        pw->setPos(QPoint(xx, yy));
        pw->setZValue(8);

        scene->addItem(pw);
    }
}

void ObjFab::createCoup(QJsonDocument json)
{
    ObjCoup *coup;
    QJsonValue ob;
    int type;
    QColor color;

    for (auto i = 0; !json[i].isUndefined() ; ++i) {
        ob = json[i];
        if (ob["installed"].toBool()) {
            type = ob["parr_type"].toInt(0);
            color = conf->color_coup.value(ob["object_owner"].toString(), "silver");
        }
        else {
            type = -1;
            color = "orange";
        }
        coup = new ObjCoup(type, color);
        coup->setData((int)Idx::label, "coup");
        coup->setData((int)Idx::o_id, ob["id"].toInt(0));
        coup->setData((int)Idx::o_name, ob["name"]);
        quint16 xx = ob["coord_x"].toInt(); if (xx < 20) xx = 20;
        quint16 yy = ob["coord_y"].toInt(); if (yy < 20) yy = 20;
        coup->setPos(QPoint(xx, yy));
        coup->setZValue(10);

        scene->addItem(coup);
    }
}

void ObjFab::createLocker(QJsonDocument json)
{
    ObjLocker *lo;
    QJsonValue ob;
    for (auto i = 0; !json[i].isUndefined() ; ++i) {
        ob = json[i];

        lo = new ObjLocker(ob["agr"].toBool() + ob["detached"].toBool() * 2,
                           {ob["name"].toString(),
                            conf->lo_status_list.at(ob["status"].toInt(4)),
                            conf->lo_status_color.at(ob["status"].toInt(4)),
                            conf->color_lo_co.value(ob["co"].toString(), "silver"),
                            ob["co"].toString(),
                            ob["object_owner"].toString()
                           });
        lo->setData((int)Idx::label, "locker");
        lo->setData((int)Idx::o_id, ob["id"].toInt(0));
        lo->setData((int)Idx::o_name, ob["name"]);
        quint16 xx = ob["coord_x"].toInt(); if (xx < 20) xx = 20;
        quint16 yy = ob["coord_y"].toInt(); if (yy < 20) yy = 20;
        lo->setPos(QPoint(xx, yy));
        lo->setZValue(9);

        scene->addItem(lo);//        static_cast<QGraphicsObject*>(lo);
    }
}

void ObjFab::createPolyline(QJsonDocument json)
{
    ObjPolyline *polyline;
    QJsonValue ob;
    QPainterPath path;
    QStringList p_list, point, param;
    for (auto i = 0; !json[i].isUndefined() ; ++i) {
        ob = json[i];
//        qDebug() << "ob"<< ob;
        path.clear();
        p_list = ob["path"].toString().split("||");
//        qDebug() << "p_list"<< p_list;
        for(int i = 0; i < p_list.count(); i++){
            point = p_list.at(i).split(",");
            if (i == 0) path.moveTo(QPointF(point.at(0).toInt(), point.at(1).toInt()));
            else        path.lineTo(QPointF(point.at(0).toInt(), point.at(1).toInt()));
        }
        param = ob["param"].toString().split(",");

        polyline = new ObjPolyline(scene);
        polyline->setPath(path);
        polyline->setData((int)Idx::label, "polyline");
        polyline->setData((int)Idx::o_id, ob["id"].toInt(0));
        polyline->setPen(QPen(QColor(ob["cabcolor"].toString()),
                              param.at(0).toInt(),
                              Qt::PenStyle(param.at(1).toInt()),
                              Qt::RoundCap,
                              Qt::RoundJoin
                              ));

        QObject::connect(polyline, &ObjPolyline::sigClick, scene, &MapScene::sigLineClick);
        polyline->setZValue(param.at(2).toInt());

        scene->addItem(polyline);
    }
}
