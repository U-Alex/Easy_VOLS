#include "mapmanager.h"
#include "ui_mapmanager.h"

#include <QImageReader>

#include "map/obj/objlocker.h"

MapManager::MapManager(Config *ref_conf, UserSession *us, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapManager),
    conf(ref_conf),
    userSession(us)
{
    ui->setupUi(this);
    ui->map_scrollArea->setVisible(false);

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
//    ui->map_frame_temp->deleteLater();                   //
    ui->map_Layout->addWidget(mapView, 0, 1);

}

MapManager::~MapManager()
{
    if (coupManager != nullptr) {
        QObject::disconnect(coupManager, &CoupManager::destroyed, this, &MapManager::welding_exit);
        coupManager->deleteLater();
    }
    mapView->deleteLater();
    objFab->deleteLater();
    scene->deleteLater();
    delete ui;
}

void MapManager::start()
{
    ui->frame_com->setEnabled(true);
    ui->map_frame_L->setEnabled(true);
    showAllObj();

    QImageReader::setAllocationLimit(conf->image_allocation_limit);
    QPixmap image(conf->map_f_name);
    map_size = image.size();
    pix_map = scene->addPixmap(image);
    pix_map->setData((int)Idx::o_type, (int)ObjType::pix_map);

    mapView->centerOn(QPointF(9000, 15378));        //
//    mapView->centerOn(QPointF(map_size.width()/2, map_size.height()/2));
//    showAllObj()
//        ui->pb_welding->click();            //
}

void MapManager::showAllObj()
{
    userSession->getData(ObjType::o_pw_cont);
    userSession->getData(ObjType::o_coup);
    userSession->getData(ObjType::o_locker);
    userSession->getData(ObjType::o_polyline);
    userSession->getData(ObjType::o_label);
}

void MapManager::on_pb_map_refresh_clicked()
{
    pwcont_upd_list.clear();
    coup_upd_list.clear();
    line_upd_id.clear();
    queue_label.clear();
    ui->pb_edit->setChecked(false);
    ui->pb_apply->setEnabled(false);
    ui->pb_link->setChecked(false);
    ui->pb_link->setEnabled(true);
    last_coup = nullptr;
//    int id = (int)Idx::label;
    foreach (QGraphicsItem *item, scene->items()) {
        if (item->data((int)Idx::o_type) != (int)ObjType::pix_map) {
            scene->removeItem(item);
            delete item;
        }
    }
    showAllObj();
}

void MapManager::on_pb_edit_pressed()
{
    ui->pb_link->setChecked(false);
//    scene->deselectItems();
//    ui->lab_coord->setText("  ");
//    ui->lab_obj_name->setText("");
}

void MapManager::on_pb_link_pressed()
{
    ui->pb_edit->setChecked(false);
//    scene->deselectItems();
//    ui->lab_coord->setText("  ");
//    ui->lab_obj_name->setText("");
}

void MapManager::on_pb_edit_toggled(bool checked)
{
    ui->map_scrollArea->setVisible(checked);
//    int conf_lab = (int)Idx::label;
//    QString lab{};
//    QStringList lab_list{"pwcont", "coup", "label"};
//    foreach (QGraphicsItem *item, scene->items()) {
//        lab = item->data(conf_lab).toString();
//        if (lab == "polyline")
//            static_cast<ObjPolyline *>(item)->setMove(checked);
//        if (lab_list.contains(lab))
//            item->setFlag(QGraphicsItem::ItemIsMovable, checked);
//    }
    QList<uint> lab_list{(int)ObjType::o_pw_cont, (int)ObjType::o_coup, (int)ObjType::o_label};
    foreach (QGraphicsItem *item, scene->items()) {
        if (item->data((int)Idx::o_type) == (int)ObjType::o_polyline)
            static_cast<ObjPolyline *>(item)->setMove(checked);
        if (lab_list.contains(item->data((int)Idx::o_type)))
            item->setFlag(QGraphicsItem::ItemIsMovable, checked);
    }
    if (fr_edit != nullptr) {
        ui->map_Layout_R->removeWidget(fr_edit);
        fr_edit->deleteLater();
        fr_edit = nullptr;
    }
    if (checked) {
        fr_edit = new MapManagerEdit(this);
//        connect(scene, &MapScene::sigBlankClick, fr_edit, &MapManagerEdit::slot_blank);
//        connect(scene, &MapScene::sigLabelClick, fr_edit, &MapManagerEdit::slot_label);
        ui->map_Layout_R->addWidget(fr_edit);
    }
}

void MapManager::on_pb_link_toggled(bool checked)
{
    ui->map_scrollArea->setVisible(checked);
    if (fr_link != nullptr) {
        ui->map_Layout_R->removeWidget(fr_link);
        fr_link->deleteLater();
        fr_link = nullptr;
    }
    if (checked) {
        fr_link = new MapManagerLink(conf, userSession, scene, this);
        QObject::connect(userSession, &UserSession::sigCoupLinks, fr_link, &MapManagerLink::slotCoupLinks);
        ui->map_Layout_R->addWidget(fr_link);
    }
}

void MapManager::on_pb_apply_clicked()
{
    ui->pb_apply->setEnabled(false);
    ui->pb_link->setEnabled(true);

    if (! pwcont_upd_list.isEmpty()) {
        userSession->setData(ObjType::o_pw_cont, pwcont_upd_list);
    }
    if (! coup_upd_list.isEmpty()) {
        userSession->setData(ObjType::o_coup, coup_upd_list);
    }
    if (! line_upd_id.isEmpty()) {
        QMap<int, QList<QVariant>>  line_upd_list;
        ObjPolyline *poly;
        foreach (QGraphicsItem *item, scene->items()) {
            if (item->data((int)Idx::o_type) == (int)ObjType::o_polyline) {
                int l_id = item->data((int)Idx::o_id).toInt();
                if (line_upd_id.contains(l_id)) {
                    poly = static_cast<ObjPolyline *>(item);
                    QPainterPath path = poly->path();
                    QStringList p_list{};
                    for(int i = 0; i < path.elementCount(); ++i){
                        QPointF point = path.elementAt(i);
                        p_list << QString("%1,%2").arg(point.toPoint().x()).arg(point.toPoint().y());
                    }
                    line_upd_list.insert(l_id, {p_list.join("||"), poly->data((int)Idx::param).toString()});
                }
            }
        }
        userSession->setData(ObjType::o_polyline, line_upd_list);
    }
qDebug() << "queue_label"<< queue_label;
    pwcont_upd_list.clear();
    coup_upd_list.clear();
    line_upd_id.clear();
    queue_label.clear();
}

void MapManager::on_pb_pix_hide_toggled(bool checked)
{
    pix_map->setVisible(checked);
}

void MapManager::on_pb_obj_hide_clicked()
{
    foreach (QGraphicsItem *item, scene->selectedItems()) {
        item->setSelected(false);
        item->setVisible(false);
    }
    if (! ui->pb_all_visible->isEnabled()) ui->pb_all_visible->setEnabled(true);
}

void MapManager::on_pb_all_visible_clicked()
{
//    ui->pb_pix_hide->setChecked(false);
    ui->pb_all_visible->setEnabled(false);
    QStringList param;
    int conf_param = (int)Idx::param;
    foreach (QGraphicsItem *item, scene->items()) {
        item->setSelected(false);
        item->setVisible(true);
        item->setData((int)Idx::VisMode, 1);
        if (item->data((int)Idx::o_type) == (int)ObjType::o_polyline) {
            ObjPolyline *poly = static_cast<ObjPolyline *>(item);
            param = poly->data(conf_param).toString().split(",");
            poly->setPen(QPen(QColor(poly->data((int)Idx::cabcolor).toString()),
                              param.at(0).toInt(),
                              Qt::PenStyle(param.at(1).toInt()),
                              Qt::RoundCap,
                              Qt::RoundJoin
                              ));
        }
    }
    scene->update();
}

//--------------------------------------------------------------------------

void MapManager::slotPwcontPress(QGraphicsItem *ref_item)
{
    if (scene->selectedItems().length() > 1)
        scene->deselectItems();
    ObjPwcont *item = static_cast<ObjPwcont *>(ref_item);
    QPoint dest_pos = item->scenePos().toPoint();
    last_pw_cont = item;
    last_pw_cont_pos = dest_pos;
    ui->lab_obj_name->setText(QString("объект: %1 <%2>")
                              .arg(item->data((int)Idx::o_name).toString())
                              .arg(item->data((int)Idx::o_id).toString()));
    ui->lab_coord->setText(QString("%1,%2")
                           .arg(dest_pos.x()).arg(dest_pos.y()));
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
                return;
            }
            pwcont_upd_list.insert(last_pw_cont->data((int)Idx::o_id).toInt(), {dest_pos.x(), dest_pos.y()});
            last_pw_cont_pos = dest_pos;
            if (! ui->pb_apply->isEnabled()) ui->pb_apply->setEnabled(true);
        }
    }
}

//--------------------------------------------------------------------------

void MapManager::slotCoupPress(QGraphicsItem *ref_item)
{
    if (scene->selectedItems().length() > 1)
        scene->deselectItems();
    ObjCoup *item = static_cast<ObjCoup *>(ref_item);
    QPoint dest_pos = item->scenePos().toPoint();
    last_coup = item;
    last_coup_pos = dest_pos;
    ui->lab_obj_name->setText(QString("муфта: %1 <%2>")
                              .arg(item->data((int)Idx::o_name).toString())
                              .arg(item->data((int)Idx::o_id).toString()));
    ui->lab_coord->setText(QString("%1,%2")
                           .arg(dest_pos.x()).arg(dest_pos.y()));
    if (fr_link != nullptr)
        fr_link->setCoup(item);
}

void MapManager::slotCoupClick(QGraphicsItem *ref_item)
{
    ObjCoup *item = static_cast<ObjCoup *>(ref_item);
    QPoint dest_pos = item->scenePos().toPoint();
    if (ui->pb_edit->isChecked()) {
        if (last_coup == item && last_coup_pos != dest_pos) {        //out of size
            if (dest_pos.x() < 20 || dest_pos.x() > map_size.width()-50 || dest_pos.y() < 20 || dest_pos.y() > map_size.height()-30) {
                item->setPos(last_coup_pos);
                dest_pos = last_coup_pos;
            }
            else {
                slotCoupMoved(dest_pos);
                last_coup_pos = dest_pos;
            }
        }
    }
    else if (coupManager != nullptr)
        coupManager->nextCoup(item->data((int)Idx::o_id).toUInt());
//        emit showWelding(item->data((int)Idx::o_id).toUInt());
}

void MapManager::slotCoupMoved(QPointF to_pos)
{
    ui->pb_apply->setEnabled(true);
    ui->pb_link->setEnabled(false);
    QString idid;
    QString last_id = last_coup->data((int)Idx::o_id).toString();
    int parr_id = last_coup->data((int)Idx::parr_id).toInt();
    int curr_type = last_coup->data((int)Idx::parr_type).toInt();
    int c_id = last_id.toInt();
//    if (! queue_coup.contains(c_id))
//        queue_coup << c_id;
    coup_upd_list.insert(c_id, {to_pos.x(), to_pos.y(), parr_id, curr_type});
    foreach (QGraphicsItem *item, scene->items()) {
        if (curr_type == 0) {
            if ((item->data((int)Idx::o_type) == (int)ObjType::o_locker) &&
                (item->data((int)Idx::o_id).toInt() == parr_id)) {
                //obj_lo *lo = static_cast<obj_lo *>(item);
                item->setPos(to_pos);
            }
        }
        if (item->data((int)Idx::o_type) == (int)ObjType::o_polyline) {
            idid = item->data((int)Idx::lineidid).toString();

            if (idid.startsWith(last_id+",")) {
                ObjPolyline *poly = static_cast<ObjPolyline *>(item);
                QPainterPath linePath = poly->path();
                linePath.setElementPositionAt(0, to_pos.x(), to_pos.y());
                poly->setPath(linePath);
                int l_id = poly->data((int)Idx::o_id).toInt();
                if (! line_upd_id.contains(l_id))
                    line_upd_id << l_id;
            }
            if (idid.endsWith(","+last_id)) {
                ObjPolyline *poly = static_cast<ObjPolyline *>(item);
                QPainterPath linePath = poly->path();
                linePath.setElementPositionAt(linePath.elementCount()-1, to_pos.x(), to_pos.y());
                poly->setPath(linePath);
                int l_id = poly->data((int)Idx::o_id).toInt();
                if (! line_upd_id.contains(l_id))
                    line_upd_id << l_id;
            }
        }
    }
}

//--------------------------------------------------------------------------

void MapManager::slotLockerClick(QGraphicsItem *ref_item)
{
    ObjLocker *item = static_cast<ObjLocker *>(ref_item);
    ui->lab_obj_name->setText(QString("объект: %1 <%2>")
                              .arg(item->data((int)Idx::o_name).toString())
                              .arg(item->data((int)Idx::o_id).toString()));
    ui->lab_coord->setText(QString("%1,%2")
                           .arg(item->pos().toPoint().x()).arg(item->pos().toPoint().y()));
}

//--------------------------------------------------------------------------

void MapManager::slotLineClick(QGraphicsItem *ref_item)
{
    ObjPolyline *item = static_cast<ObjPolyline *>(ref_item);
    ui->lab_obj_name->setText(QString("кабель: %1")
                              .arg(item->data((int)Idx::cabtype).toString()));
                              /*.arg(item->data((int)Idx::o_id).toString()));*/
//    ui->lab_obj_name->setText("кабель: " + item->data((int)Idx::cabtype).toString() +" <"+ item->data((int)Idx::o_id).toString());
//                          " | " + item->data(conf->Obj.indexOf("LineIdId")).toString() +
//                          " | " + item->data(conf->Obj.indexOf("LineCNCN")).toString() + ">");
//    qDebug()<<item->data((int)Idx::lineidid)<<item->data((int)Idx::linecncn);
    ui->lab_coord->setText("  ");
    if (ui->pb_edit->isChecked()) {
        fr_edit->cableClick(item);
        int l_id = item->data((int)Idx::o_id).toInt();
        if (! line_upd_id.contains(l_id))
            line_upd_id << l_id;
        if (! ui->pb_apply->isEnabled())
            ui->pb_apply->setEnabled(true);
    }
    if (scene->selectedItems().count() == 1) {
        if (item->data(0).toInt() != scene->selectedItems().at(0)->data(0).toInt()) {
            scene->deselectItems();
            item->setSelected(true);
        }
    }
    else {
        scene->deselectItems();
        item->setSelected(true);
    }
}

//--------------------------------------------------------------------------

void MapManager::slotBlankClick(QPoint pos)
{
    ui->lab_coord->setText(QString("%1,%2").arg(pos.x()).arg(pos.y()));
    ui->lab_obj_name->clear();
}

void MapManager::slotLabelClick(QGraphicsTextItem *item)
{
    if (ui->pb_edit->isChecked()) {
        int lab_id = item->data((int)Idx::o_id).toInt();
        if (! queue_label.contains(lab_id)) {
            queue_label << lab_id;
        }
        if (! ui->pb_apply->isEnabled()) ui->pb_apply->setEnabled(true);
    }
}

//--------------------------------------------------------------------------

void MapManager::on_pb_welding_toggled(bool checked)
{
    if (checked) {
        if (coupManager == nullptr) coupManager = new CoupManager(conf, userSession, this);
        coupManager->setAttribute(Qt::WA_DeleteOnClose, 1);
        coupManager->setWindowFlags(Qt::Window);
        QObject::connect(coupManager, &CoupManager::sigToMapCoup, mapView, &MapView::slotCoupOnCenter);
        QObject::connect(userSession, &UserSession::sigShowHopData, this, &MapManager::slotShowHopData);
        QObject::connect(coupManager, &CoupManager::destroyed, this, &MapManager::welding_exit);
        coupManager->show();
    } else if (coupManager != nullptr) {
        coupManager->deleteLater();
        coupManager = nullptr;
    }
}

void MapManager::welding_exit()
{
    coupManager = nullptr;
    ui->pb_welding->setChecked(false);
}

void MapManager::slotShowHopData(/*uint c_p_id, */QJsonDocument json)
{
//    qDebug()<<"MapManager::slotShowHop"<< c_p_id << json;
    QVector<int> coup_id, cab_len;
    QStringList idid, cncn;
    QVector<QPair<QString, QString>> idid_list, cncn_list;
    QVector<int> coup_id_list, cab_num_list, cab_len_list;
    auto _cil = json["coup_id_list"], _cnl = json["cab_num_list"], _cll = json["cab_len_list"];
    for (auto i = 0; !_cil[i].isUndefined(); ++i) coup_id_list << _cil[i].toInt();
    for (auto i = 0; !_cnl[i].isUndefined(); ++i) cab_num_list << _cnl[i].toInt();
    for (auto i = 0; !_cll[i].isUndefined(); ++i) cab_len_list << _cll[i].toInt();
    for (int idx = 0; idx < coup_id_list.size(); idx += 2) {
        if (idx % 2 == 0) {
            coup_id << coup_id_list[idx+1];
            cab_len << cab_len_list[idx+1];
        }
        idid_list << QPair<QString, QString>(QString("%1").arg(coup_id_list[idx]),
                                             QString("%1").arg(coup_id_list[idx+1]));
        cncn_list << QPair<QString, QString>(QString("%1").arg(cab_num_list[idx]),
                                             QString("%1").arg(cab_num_list[idx+1]));
    }
//qDebug()<<coup_id<<cab_len<<idid<<cncn;
    foreach (QGraphicsItem *item, scene->items()) {
        if (item->data((int)Idx::o_type) == (int)ObjType::o_coup) {
            if (coup_id.contains(item->data((int)Idx::o_id).toInt()))
                item->setData((int)Idx::VisMode, 2);
            else
                item->setData((int)Idx::VisMode, 0);
        }
        if (item->data((int)Idx::o_type) == (int)ObjType::o_polyline) {
            idid = item->data((int)Idx::lineidid).toString().split(",");
            cncn = item->data((int)Idx::linecncn).toString().split(",");
            ObjPolyline *poly = static_cast<ObjPolyline *>(item);
            bool found = false;
            for (int idx = 0; idx < idid_list.size(); ++idx) {
                if ((idid.at(0) == idid_list.at(idx).first && idid.at(1) == idid_list.at(idx).second &&
                     cncn.at(0) == cncn_list.at(idx).first && cncn.at(1) == cncn_list.at(idx).second) ||
                    (idid.at(1) == idid_list.at(idx).first && idid.at(0) == idid_list.at(idx).second &&
                     cncn.at(1) == cncn_list.at(idx).first && cncn.at(0) == cncn_list.at(idx).second)
                   ) {
                    found = true;
                    break;
                }
            }
            if (found)
                poly->setPen(QPen(QColor(conf->cab_color.value("!")),
                                  conf->cab_width.value("!").toUInt(),
                                  Qt::SolidLine,
                                  Qt::RoundCap,
                                  Qt::RoundJoin
                                 ));
            else
                poly->setPen(QPen(QColor(conf->cab_color.value("0")),
                                  conf->cab_width.value("0").toUInt(),
                                  Qt::SolidLine,
                                  Qt::RoundCap,
                                  Qt::RoundJoin
                                 ));
        }
    }
    ui->pb_all_visible->setEnabled(true);
    scene->update();
}

