#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <QWidget>
#include <QGraphicsPixmapItem>

#include "config.h"
#include "map/mapscene.h"
#include "map/mapview.h"
#include "b_logic/objfab.h"
#include "b_logic/usersession.h"
#include "map/obj/objpwcont.h"
#include "map/obj/objcoup.h"


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

    ObjPwcont      *last_pw_cont = nullptr;
    QPoint          last_pw_cont_pos;

    ObjCoup        *last_coup = nullptr;
    QPoint          last_coup_pos;

    QVector<int>    queue_pw_cont, queue_coup;
    QVector<int>    queue_line, queue_label;


private slots:
    void showAllObj();
    void on_pb_map_refresh_clicked();
    void on_pb_edit_toggled(bool checked);

    void slotPwcontPress(QGraphicsItem *);
    void slotPwcontClick(QGraphicsItem *);

    void slotCoupPress(QGraphicsItem *);
    void slotCoupClick(QGraphicsItem *);
    void slotCoupMoved(QPointF);

    void slotLockerClick(QGraphicsItem *);

    void slotLineClick(QGraphicsItem *);

    void slotBlankClick(QPoint);
    void slotLabelClick(QGraphicsTextItem *);


};


#endif // MAPMANAGER_H
