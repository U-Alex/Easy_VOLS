#include "mapmanagerlink.h"
#include "ui_mapmanagerlink.h"

#include "map/obj/obj.h"

MapManagerLink::MapManagerLink(Config *ref_conf, UserSession *us, MapScene *scene, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MapManagerLink),
    conf(ref_conf),
    userSession(us),
    scene(scene)
{
    ui->setupUi(this);
}

MapManagerLink::~MapManagerLink()
{
    delete ui;
}

void MapManagerLink::setCoup(ObjCoup *ref_coup)
{
    coup = ref_coup;
    coup_id = coup->data((int)Idx::o_id).toUInt();
    userSession->getCoupLinks(coup->data((int)Idx::o_id).toUInt());
}

void MapManagerLink::slotCoupLinks(/*uint c_id, */QJsonDocument json/*, bool one_link*/)
{
    for (auto &ch: this->children()) {
        if (ch->objectName().startsWith("d_"))
        ch->deleteLater();
    }
    for (auto &ch: ui->verticalLayout_2->children()) {
        ch->deleteLater();
    }
    current_links.clear();
//    qDebug() << "current_links"<< current_links.count();
    QVector<uint> it;
    QStringList idid, cncn;
    foreach (QGraphicsItem *item, scene->items()) {
        if (item->data((int)Idx::label).toString() == "polyline") {
            it << item->data((int)Idx::o_id).toUInt();
            idid = item->data((int)Idx::lineidid).toString().split(",");
            it << idid.at(0).toUInt() << idid.at(1).toUInt();
            cncn = item->data((int)Idx::linecncn).toString().split(",");
            it << cncn.at(0).toUInt() << cncn.at(1).toUInt();
            current_links.append(it);
            it.clear();
        }
    }
    /*if (!one_link) */coup_links.clear();
    QJsonValue ob;
    QMap<QString, QVariant> rec;
    for (auto i = 0; !json[i].isUndefined() ; ++i) {
        ob = json[i];
//        qDebug() << "ob"<< ob;
        rec.insert("ext_coup_id", ob["ext_coup_id"].toInt());
        rec.insert("ext_coup_name", ob["ext_coup_name"].toString());
        rec.insert("ext_cable_num", ob["ext_cable_num"].toInt());
        rec.insert("ext_coup_x", ob["ext_coup_x"].toInt());
        rec.insert("ext_coup_y", ob["ext_coup_y"].toInt());
        rec.insert("cable_num", ob["cable_num"].toInt());
        rec.insert("cable_type", ob["cable_type"].toInt());
        rec.insert("cable_name", ob["cable_name"].toString());
        rec.insert("cable_capa", ob["cable_capa"].toInt());
        coup_links.append(rec);
        rec.clear();
    }
//    qDebug() << "current_links"<< current_links.count() << "coup_links" << coup_links.count();
    createButCabList();
}

void MapManagerLink::createButCabList()
{
    QLabel *lbl, *lbl2;
    QPushButton *pcmd1, *pcmd2;
    QGridLayout *grid_Layout;
    foreach (auto &row, coup_links) {
        lbl = new QLabel(row["cable_num"].toString() + ":  " + row["ext_coup_name"].toString());
        lbl2 = new QLabel(row["cable_name"].toString());
        lbl2->setStyleSheet("font-size: 10px;");
        lbl->setObjectName("d_lbl_");
        lbl2->setObjectName("d_lbl_");
//        pcmd1 = new QPushButton("◀---  ▲  ---▶");
        pcmd1 = new QPushButton(QString("◀----  %1  ----▶").arg(row["cable_capa"].toString()));
        pcmd1->setObjectName("d_");
        pcmd1->setFocusPolicy(Qt::NoFocus);
        pcmd1->setMinimumSize(120,24); pcmd1->setMaximumSize(120,24);
        pcmd2 = new QPushButton(" X ");
        pcmd2->setObjectName("d_");
        pcmd2->setFocusPolicy(Qt::NoFocus);
        pcmd2->setMinimumSize(40,24); pcmd2->setMaximumSize(40,24);
        uint exists = linkExists(coup_id,
                                 row["ext_coup_id"].toUInt(),
                                 {row["cable_num"].toUInt(), row["ext_cable_num"].toUInt()});
        if (exists) {
            pcmd1->setEnabled(false);
            pcmd2->setObjectName(QString("d_pcmd2_%1").arg(exists));
            connect(pcmd2, &QPushButton::clicked, this, &MapManagerLink::butXClicked);
        } else {
            pcmd2->setEnabled(false);
            pcmd1->setObjectName(QString("d_pcmd1_%1").arg(coup_links.indexOf(row)));
            connect(pcmd1, &QPushButton::pressed, this, &MapManagerLink::butVClicked);
        }
        grid_Layout = new QGridLayout;
        grid_Layout->addWidget(lbl, 0, 0, 1, 2);
        grid_Layout->addWidget(lbl2, 1, 0, 1, 2);
        grid_Layout->addWidget(pcmd1, 2, 0);
        grid_Layout->addWidget(pcmd2, 2, 1);
        ui->verticalLayout_2->addLayout(grid_Layout);
    }
}

uint MapManagerLink::linkExists(uint c_id, uint dest_c_id, QPair<uint, uint> cn)
{
    foreach (auto &rec, current_links) {
        if ((rec.at(1) == c_id && rec.at(2) == dest_c_id && rec.at(3) == cn.first && rec.at(4) == cn.second) ||
            (rec.at(2) == c_id && rec.at(1) == dest_c_id && rec.at(4) == cn.first && rec.at(3) == cn.second)
           )
            return rec.at(0);
    }
    return 0;
}

void MapManagerLink::butVClicked()
{
    QPushButton *but = static_cast<QPushButton*>(sender());
    but->setEnabled(false);
    QMap<QString, QVariant> new_link = coup_links.at(but->objectName().split("_").at(2).toUInt());
    new_link.insert("coup_id", coup->data((int)Idx::o_id).toUInt());
    new_link.insert("coup_x", coup->x());
    new_link.insert("coup_y", coup->y());
    new_link.insert("cab_color", conf->cab_color.value(new_link.value("cable_capa").toString()));
    new_link.insert("cab_width", conf->cab_width.value(new_link.value("cable_capa").toString()));
//    qDebug() << new_link;

    userSession->createLink(/*0, */new_link);
}

void MapManagerLink::butXClicked()
{
    QPushButton *but = static_cast<QPushButton*>(sender());
    but->setEnabled(false);
    QString line_id = but->objectName().split("_").at(2);
    userSession->deleteLink(line_id.toUInt());
    foreach (QGraphicsItem *item, scene->items()) {
        if (item->data((int)Idx::label).toString() == "polyline") {
            if (item->data((int)Idx::o_id).toString() == line_id) {
                scene->removeItem(item);
                delete item;
            }
        }
    }
}

