#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <QWidget>
#include <QGraphicsPixmapItem>

#include "map/mapscene.h"
#include "map/mapview.h"
//#include "map/obj/objfab.h"
//#include "map/obj/objcoup.h"    //
#include "b_logic/usersession.h"

namespace Ui { class MapManager; }

class MapManager : public QWidget
{
    Q_OBJECT

public:
    explicit MapManager(QWidget *parent, UserSession *us);
    ~MapManager();

    MapScene       *scene;
    MapView        *mapView;
//    ObjFab         *objFab;

public slots:
    void objRecieve(ObjType, uint, QList<QGraphicsObject*>);


private:
    Ui::MapManager *ui;
    UserSession    *userSession;
    QGraphicsPixmapItem *pix_map;
};


#endif // MAPMANAGER_H
