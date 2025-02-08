#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <QWidget>
#include <QGraphicsPixmapItem>

#include "config.h"
#include "map/mapscene.h"
#include "map/mapview.h"
#include "b_logic/objfab.h"
#include "b_logic/usersession.h"

namespace Ui { class MapManager; }

class MapManager : public QWidget
{
    Q_OBJECT
public:
    explicit MapManager(QWidget *parent, Config *ref_conf, UserSession *us);
    ~MapManager();

    MapScene       *scene;
    MapView        *mapView;
    ObjFab         *objFab;

private:
    Ui::MapManager *ui;
    Config         *conf;
    UserSession    *userSession;
    QGraphicsPixmapItem *pix_map;
    QSize           map_size;

private slots:
    void showAllObj();
    void on_pb_map_refresh_clicked();







};


#endif // MAPMANAGER_H
