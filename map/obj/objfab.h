#ifndef OBJFAB_H
#define OBJFAB_H

#include <QGraphicsObject>
#include <QJsonDocument>
#include "map/mapscene.h"
#include "map/obj/obj.h"


class ObjFab : public QObject
{
    Q_OBJECT
public:
    explicit ObjFab(MapScene *scene, QObject *parent = nullptr);


public slots:
    void fromDataToObj(ObjType, uint, QJsonDocument);

private:
    MapScene       *scene;

private slots:
    void createCoup(QJsonDocument json);
    void createLocker(QJsonDocument json);
    void createPwcont(QJsonDocument json);

};

#endif // OBJFAB_H

//    Obj="Id❚ParrentId❚Name❚LineIdId❚LineCNCN❚Type❚Label❚VisMode❚CabType❚CabColor❚----------❚LabColor1❚LabColor2❚Param"
//    //   0  1    2    3        4        5         6     7       8       9        10         11        12        13
