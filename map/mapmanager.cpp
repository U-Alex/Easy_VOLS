#include "mapmanager.h"
#include "ui_mapmanager.h"

#include "map/obj/objpolyline.h"

#include <QImageReader>

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
    QObject::connect(scene, &MapScene::sigBlankClick, this, &MapManager::slotBlankClick);
    QObject::connect(scene, &MapScene::sigLabelClick, this, &MapManager::slotLabelClick);
    QObject::connect(scene, &MapScene::sigPwcontPress, this, &MapManager::slotPwcontPress);
    QObject::connect(scene, &MapScene::sigPwcontRelease, this, &MapManager::slotPwcontClick);
    QObject::connect(scene, &MapScene::sigCoupPress, this, &MapManager::slotCoupPress);
    QObject::connect(scene, &MapScene::sigCoupRelease, this, &MapManager::slotCoupClick);
    QObject::connect(scene, &MapScene::sigLockerPress, this, &MapManager::slotLockerClick);
    QObject::connect(scene, &MapScene::sigLineClick, this, &MapManager::slotLineClick);

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

void MapManager::on_pb_edit_toggled(bool checked)
{
    int conf_lab = (int)Idx::label;
    foreach(QGraphicsItem *item, scene->items()) {
        if (item->data(conf_lab).toString() == "pwcont") {
            item->setFlag(QGraphicsItem::ItemIsMovable, checked);
        }
        if (item->data(conf_lab).toString() == "coup") {
            item->setFlag(QGraphicsItem::ItemIsMovable, checked);
        }
        if (item->data(conf_lab).toString() == "polyline") {
            ObjPolyline *poly = static_cast<ObjPolyline *>(item);
            poly->setMove(checked);
        }
//        if (item->data(conf_lab).toString() == "label") {
//            item->setFlag(QGraphicsItem::ItemIsMovable, checked);
//        }
    }
//    ui->scrollArea->setVisible(checked);
//    if (fr_edit != nullptr) {
//        ui->R_Layout->removeWidget(fr_edit);
//        fr_edit->deleteLater();
//        fr_edit = nullptr;
//    }
//    if (checked) {
//        fr_edit = new Map_mgr_edit(conf, orm, scene, this);
//        connect(scene, &Map_scene::sig_blank_click, fr_edit, &Map_mgr_edit::slot_blank);
//        connect(scene, &Map_scene::sig_label_click, fr_edit, &Map_mgr_edit::slot_label);
//        //connect(scene, &Map_scene::label_click, this, &Map_mgr::slot_label_click);
//        connect(this, &Map_mgr::cable_click, fr_edit, &Map_mgr_edit::slot_cable);
//        ui->R_Layout->addWidget(fr_edit);
//    }
}
//--------------------------------------------------------------------------

void MapManager::slotPwcontPress(QGraphicsItem *ref_item)
{
    if (scene->selectedItems().length() > 1) {
        scene->deselectItems();
    }
    ObjPwcont *item = static_cast<ObjPwcont *>(ref_item);
    QPoint dest_pos = item->scenePos().toPoint();
    last_pw_cont = item;
    last_pw_cont_pos = dest_pos;
    ui->lab_obj_name->setText("объект: " + item->data((int)Idx::o_name).toString() + " <" + item->data((int)Idx::o_id).toString() + ">");
    ui->lab_coord->setText(QString("%1,%2").arg(dest_pos.x()).arg(dest_pos.y()));
}

void MapManager::slotPwcontClick(QGraphicsItem *ref_item)
{
    ObjPwcont *item = static_cast<ObjPwcont *>(ref_item);
    QPoint dest_pos = item->scenePos().toPoint();
    if (ui->pb_edit->isChecked()) {
        if (last_pw_cont == item && last_pw_cont_pos != dest_pos) {        //out of size
            if (dest_pos.x() < 20 || dest_pos.x() > map_size.width()-50 || dest_pos.y() < 20 || dest_pos.y() > map_size.height()-30) {
                item->setPos(last_pw_cont_pos);
                dest_pos = last_pw_cont_pos;
            }
            else {
                int c_id = last_pw_cont->data((int)Idx::o_id).toInt();
                if (! queue_pw_cont.contains(c_id)) {
                    queue_pw_cont << c_id;
                }
                last_pw_cont_pos = dest_pos;
                if (! ui->pb_apply->isEnabled()) ui->pb_apply->setEnabled(true);
            }
        }
    }
}

//--------------------------------------------------------------------------

void MapManager::slotCoupPress(QGraphicsItem *ref_item)
{qDebug() << "slotCoupPress"<< ref_item;

}
void MapManager::slotCoupClick(QGraphicsItem *ref_item)
{qDebug() << "slotCoupClick"<< ref_item;

}
void MapManager::slotCoupMoved(QPointF to_pos)
{qDebug() << "slotCoupMoved"<< to_pos;

}

//--------------------------------------------------------------------------


void MapManager::slotLockerClick(QGraphicsItem *ref_item)
{qDebug() << "slotLockerClick"<< ref_item;

}

//--------------------------------------------------------------------------

void MapManager::slotLineClick(QGraphicsItem *ref_item)
{qDebug() << "slotLineClick"<< ref_item;

}

//--------------------------------------------------------------------------

void MapManager::slotBlankClick(QPoint to_pos)
{qDebug() << "slotBlankClick"<< to_pos;

}
void MapManager::slotLabelClick(QGraphicsTextItem *ref_item)
{qDebug() << "slotLabelClick"<< ref_item;

}

//--------------------------------------------------------------------------



