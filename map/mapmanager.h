#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <QWidget>
#include <QGraphicsPixmapItem>

#include "config.h"
#include "map/mapscene.h"
#include "map/mapview.h"
#include "map/mapmanageredit.h"
#include "map/mapmanagerlink.h"
#include "b_logic/objfab.h"
#include "b_logic/usersession.h"
#include "map/obj/objpwcont.h"
#include "map/obj/objcoup.h"
#include "coup/coupmanager.h"

namespace Ui { class MapManager; }

class MapManager : public QWidget
{
    Q_OBJECT
public:
    explicit MapManager(Config *ref_conf, UserSession *us, QWidget *parent = nullptr);
    ~MapManager();

//    MapScene       *scene;
//    MapView        *mapView;
//    ObjFab         *objFab;

public slots:
//    void toCoupOnCenter(int coup_id);
//    void showItemsHop(int p_id);

signals:
//    void cableClick(ObjPolyline *);
//    void showWelding(uint coup_id);

private:
    Ui::MapManager *ui;
    Config         *conf = nullptr;
    UserSession    *userSession = nullptr;
    MapScene       *scene = nullptr;
    MapView        *mapView = nullptr;
    ObjFab         *objFab = nullptr;
    CoupManager    *coupManager = nullptr;

    MapManagerEdit *fr_edit = nullptr;
    MapManagerLink *fr_link = nullptr;

    QGraphicsPixmapItem *pix_map;
    QSize           map_size;

    ObjPwcont      *last_pw_cont = nullptr;
    QPoint          last_pw_cont_pos;

    ObjCoup        *last_coup = nullptr;
    QPoint          last_coup_pos;

    QVector<int>    queue_label;//
    QMap<int, QList<QVariant>>  pwcont_upd_list, coup_upd_list;
    QVector<int>    line_upd_id;

private slots:
    void showAllObj();
    void on_pb_edit_pressed();
    void on_pb_link_pressed();
    void on_pb_edit_toggled(bool checked);
    void on_pb_link_toggled(bool checked);
    void on_pb_apply_clicked();
    void on_pb_map_refresh_clicked();
    void on_pb_pix_hide_toggled(bool checked);
    void on_pb_obj_hide_clicked();
    void on_pb_all_visible_clicked();

    void slotPwcontPress(QGraphicsItem *);
    void slotPwcontClick(QGraphicsItem *);
    void slotCoupPress(QGraphicsItem *);
    void slotCoupClick(QGraphicsItem *);
    void slotCoupMoved(QPointF);
    void slotLockerClick(QGraphicsItem *);
    void slotLineClick(QGraphicsItem *);
    void slotBlankClick(QPoint);
    void slotLabelClick(QGraphicsTextItem *);

    void on_pb_welding_toggled(bool checked);
    void welding_exit();
};


#endif // MAPMANAGER_H
