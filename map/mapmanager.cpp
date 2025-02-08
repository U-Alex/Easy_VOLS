#include "mapmanager.h"
#include "ui_mapmanager.h"

#include <QImageReader>
//#include <QUrlQuery>        //
//#include "b_logic/restaccessmanager.h"   //
//#include "map/obj/objfab.h"
//#include "map/obj/obj.h"

MapManager::MapManager(QWidget *parent, Config *ref_conf, UserSession *us) :
    QWidget(parent),
    ui(new Ui::MapManager),
    conf(ref_conf),
    userSession(us)
{
    ui->setupUi(this);

//    ui->map_scrollArea->setVisible(false);
//    ui->map_frame_L->setVisible(false);

    scene = new MapScene(this);
    objFab = new ObjFab(conf, scene);
    QObject::connect(userSession, &UserSession::sigDataToObj, objFab, &ObjFab::slotDataToObj);
    mapView = new MapView(this);
    mapView->setScene(scene);
    ui->map_frame_temp->deleteLater();                   //
    ui->map_Layout->addWidget(mapView, 0, 1);

    QImageReader::setAllocationLimit(2048);
    QPixmap image(conf->map_f_name);
    map_size = image.size();
    pix_map = scene->addPixmap(image);
    pix_map ->setData((int)Idx::label, "pix_map");

    mapView->centerOn(QPointF(9000, 15378));        //
    showAllObj();

}

MapManager::~MapManager()
{
    delete ui;
}

void MapManager::showAllObj()
{
    userSession->getData(ObjType::o_pw_cont);
    userSession->getData(ObjType::o_coup);
    userSession->getData(ObjType::o_locker);
    userSession->getData(ObjType::o_polyline);
}

void MapManager::on_pb_map_refresh_clicked()
{
    int idx = (int)Idx::label;
    foreach(QGraphicsItem *item, scene->items()) {
        if (item->data(idx).toString() != "pix_map") {
            scene->removeItem(item);
            delete item;
        }
    }
qDebug() << "all items removed";
    showAllObj();
}

