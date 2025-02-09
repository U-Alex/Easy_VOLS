#ifndef OBJFAB_H
#define OBJFAB_H

#include <QGraphicsObject>
#include <QJsonDocument>

#include "config.h"
#include "map/mapscene.h"
#include "map/obj/obj.h"


class ObjFab : public QObject
{
    Q_OBJECT
public:
    explicit ObjFab(Config *ref_conf, MapScene *scene, QObject *parent = nullptr);


public slots:
    void slotDataToObj(ObjType, uint, QJsonDocument);

private:
    Config         *conf;
    MapScene       *scene;

private slots:
    void createPwcont(QJsonDocument json);
    void createCoup(QJsonDocument json);
    void createLocker(QJsonDocument json);
    void createPolyline(QJsonDocument json);

};

#endif // OBJFAB_H

