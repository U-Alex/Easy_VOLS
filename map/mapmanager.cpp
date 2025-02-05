#include "mapmanager.h"
#include "ui_mapmanager.h"

#include <QImageReader>
//#include <QUrlQuery>        //
//#include "b_logic/restaccessmanager.h"   //
//#include "map/obj/objfab.h"

MapManager::MapManager(QWidget *parent, UserSession *us) :
    QWidget(parent),
    ui(new Ui::MapManager),
    userSession(us)
{
    ui->setupUi(this);

//    ui->map_scrollArea->setVisible(false);
//    ui->map_frame_L->setVisible(false);

    scene = new MapScene(this);
    mapView = new MapView(this);
//    objFab = new ObjFab();                          //
    mapView->setScene(scene);
    ui->map_frame_temp->deleteLater();                   //
    ui->map_Layout->addWidget(mapView, 0, 1);


    QImageReader::setAllocationLimit(2048);
    QPixmap image("images/map1.png");
    pix_map = scene->addPixmap(image);
//    pix_map ->setData(6, "pix_map");              //

    mapView->centerOn(QPointF(9000, 15378));        //





    userSession->getObj(ObjType::o_coup);
    userSession->getObj(ObjType::o_coup, 1);
//    userSession->getObj(ObjType::o_coup, 23);

}

MapManager::~MapManager()
{
    delete ui;
}

void MapManager::objRecieve(ObjType objType, uint id, QList<QGraphicsObject*> list)
{
//qDebug() << "objRecieve " << id  << list;
//    switch(objType) {
//        case ObjType::o_pw_cont:
//            break;
//        case ObjType::o_locker:
//            break;
//        case ObjType::o_coup:
//            break;
//    }
    for (auto &coup: list) {
//        scene->addItem(static_cast<ObjCoup*>(coup));
        scene->addItem(coup);
    }
}
