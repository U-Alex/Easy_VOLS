#ifndef OBJFAB_H
#define OBJFAB_H

#include <QGraphicsObject>
#include <QJsonDocument>

//enum class Idx {
//    o_id, parrent_id, parr_type, o_name, o_type
//};


class ObjFab
{
//    Q_OBJECT
public:
    explicit ObjFab();

    QList<QGraphicsObject*> createCoup(QJsonDocument json);
    QList<QGraphicsObject*> createLocker(QJsonDocument json);


};

#endif // OBJFAB_H

//    Obj="Id❚ParrentId❚Name❚LineIdId❚LineCNCN❚Type❚Label❚VisMode❚CabType❚CabColor❚----------❚LabColor1❚LabColor2❚Param"
//    //   0  1    2    3        4        5         6     7       8       9        10         11        12        13
