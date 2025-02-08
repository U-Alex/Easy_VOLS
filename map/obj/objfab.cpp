#include "map/obj/objfab.h"
#include "map/obj/obj.h"
#include "map/obj/objcoup.h"
#include "map/obj/objlocker.h"
#include "map/obj/objpwcont.h"


ObjFab::ObjFab(MapScene *scene, QObject *parent) :
    QObject(parent),
    scene(scene)
{}

void ObjFab::fromDataToObj(ObjType objType, uint id, QJsonDocument json)
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
        }
    }
}

void ObjFab::createCoup(QJsonDocument json)
{
    ObjCoup *coup;
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
                            "в работе",
                            "orange",
                            "yellow",
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


