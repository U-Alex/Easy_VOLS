#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <QWidget>
#include <QGraphicsPixmapItem>

#include "mapscene.h"
#include "mapview.h"

namespace Ui { class MapManager; }

class MapManager : public QWidget
{
    Q_OBJECT

public:
    explicit MapManager(QWidget *parent);
    ~MapManager();

    MapScene       *scene;
    MapView        *mapView;

private:
    Ui::MapManager *ui;

    QGraphicsPixmapItem *pix_map;
};

#endif // MAPMANAGER_H
