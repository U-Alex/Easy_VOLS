#include "couppaint.h"
#include "ui_couppaint.h"

#include <QPainter>
#include <QPushButton>
#include <QPen>

CoupPaint::CoupPaint(Config *ref_conf, uint c_id, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CoupPaint),
    conf(ref_conf),
    coup_id(c_id)
{
    ui->setupUi(this);
}

CoupPaint::~CoupPaint()
{
    delete ui;
}

void CoupPaint::slotCoupPaint(uint c_id, QJsonDocument json)
{
    if (coup_id != c_id) return;
    else                 coup_id = 0;
//    qDebug() << "slotCoupPaint c_id" << c_id;
    printHead(c_id, json["cur_coup"], json["coup_parr"]);

    auto _cp = json["coup_ports"];
    for (auto i = 0; !_cp[i].isUndefined() ; ++i) {
        coup_ports << _cp[i].toObject();
    }
//qDebug() << "coup_ports"<< coup_ports;
    auto _cl = json["cab_links"];
    for (auto i = 0; !_cl[i].isUndefined() ; ++i) {
        cab_links << _cl[i].toObject();
    }
    std::sort(cab_links.begin(),cab_links.end(),[]
                (const QJsonObject& a, const QJsonObject& b)->bool{return a["cab_capa"].toInt() > b["cab_capa"].toInt();});
//qDebug() << "cab_links"<< cab_links.length();
    auto _crp = json["cross_p"];
    for (auto i = 0; !_crp[i].isUndefined() ; ++i) {
        cross_p.insert(_crp[i][0].toInt(), {_crp[i][1].toInt(), _crp[i][2].toString()});
    }
//qDebug() << "cross_p.size()"<< cross_p.size();

    prepareCab();
}

void CoupPaint::printHead(uint c_id, QJsonValue cur_coup, QJsonValue coup_parr)
{
    QString info = QString("<%1> м.: %2 (%3) %4")
            .arg(c_id).arg(cur_coup["name"].toString()).arg(cur_coup["name_type"].toString()).arg(coup_parr.toString());
    QString info2 = QString("расп.: %1 <br>прим.: %2 <br>дата установки: %3")
            .arg(cur_coup["rasp"].toString()).arg(cur_coup["prim"].toString()).arg(cur_coup["date_ent"].toString());
    ui->label_head->setText(info);
    ui->label_head->setToolTip(info2);
}

void CoupPaint::prepareCab()
{
    QList<int> v_slot = {0,0};
    short v_pos;
    for (int idx{}; idx < cab_links.length(); ++idx) {
        if (v_slot.at(0) > v_slot.at(1)) {
            v_pos = 1;
            cab_slot_R << cab_links[idx];
        } else {
            v_pos = 0;
            cab_slot_L << cab_links[idx];
        }
        v_slot[v_pos] += 2;
        v_slot[v_pos] += cab_links[idx]["cab_capa"].toInt();
    }
//qDebug() << "v_slot"<< v_slot;
    postCabList();
    createBut();
    createLinks();
}

void CoupPaint::postCabList()
{
    QStringList res_l, res_r;
    for (auto &rec : cab_slot_L) res_l << QString("%1").arg(rec["cable_num"].toInt());
    for (auto &rec : cab_slot_R) res_r << QString("%1").arg(rec["cable_num"].toInt());
    emit sigNextCoupExt(res_l, res_r);
}

void CoupPaint::createBut()
{
    ports.clear();
    int             curr_len, curr_num;
    int             mod_s = 0, p_id = 0;
    QString         curr_type;
    QList<int>      v_slot = {-1,-1};
    QPushButton    *pcmd;
    QList<QString>  foll = {"<<<",">>>"};
    QLabel         *lbl_ext, *lbl_f, *lbl_cr, *lbl_m = nullptr;
    QPoint          but_xy, lab_xy, cross_xy;

    for (short v_pos = 0; v_pos < 2; ++v_pos) {
        if (v_pos == 0) curr_len = cab_slot_L.length();
        else            curr_len = cab_slot_R.length();
//qDebug() << "v_slot---" << v_pos;
        for (int ncab = 0; ncab < curr_len; ++ncab) {
            mod_s = 0;
            if (v_pos == 0) {curr_num = cab_slot_L[ncab]["cable_num"].toInt();
                             curr_type = cab_slot_L[ncab]["cab_name"].toString();}
            else            {curr_num = cab_slot_R[ncab]["cable_num"].toInt();
                             curr_type = cab_slot_R[ncab]["cab_name"].toString();}
//qDebug() << curr_num << curr_type;
            QList<QJsonObject> coup_pn;
            foreach (QJsonObject row, coup_ports) {         //    TODO ->for ( : )
                if (row["cable_num"].toInt() == curr_num)
                    coup_pn << row;
            }
            v_slot[v_pos] += 2;

            int ext_coup_id; QString ext_coup_name;
            if (v_pos == 0) {
                ext_coup_id = cab_slot_L.at(ncab)["id"].toInt(); ext_coup_name = cab_slot_L.at(ncab)["name"].toString();
            } else {
                ext_coup_id = cab_slot_R.at(ncab)["id"].toInt(); ext_coup_name = cab_slot_R.at(ncab)["name"].toString();
            }

            pcmd = new QPushButton(foll[v_pos], this);
            pcmd->setObjectName(QString("but_c_ext_coup_%1").arg(ext_coup_id));
            pcmd->setToolTip("переход в соседнюю муфту");
            pcmd->setFocusPolicy(Qt::NoFocus);
            pcmd->setMinimumSize(conf->but_ext_coup_size); pcmd->setMaximumSize(conf->but_ext_coup_size);
            pcmd->move(conf->but_ext_coup_H_offset[v_pos], conf->but_V_offset * v_slot[v_pos]);
            pcmd->setVisible(true);
            pcmd->setStyleSheet(QString("background-color: %1").arg("silver"));
            QObject::connect(pcmd, SIGNAL(clicked(bool)), this, SLOT(but_ext_coup_clicked()));

            lbl_ext = new QLabel(ext_coup_name, this);
            lbl_ext->setObjectName(QString("lab_ext_coup_%1").arg(p_id));
            lbl_ext->setFixedSize(conf->lab_ext_coup_size);
            lbl_ext->setAlignment(Qt::AlignHCenter);
//            lbl_ext->setStyleSheet(QString("background-color: %1").arg("#eff0f1"));
            lbl_ext->move(QPoint(conf->lab_ext_coup_H_offset[v_pos], conf->but_V_offset * v_slot[v_pos] + 1));
            lbl_ext->setVisible(true);

            ++v_slot[v_pos];

            pcmd = new QPushButton(curr_type, this);
            pcmd->setObjectName(QString("but_c_ab_%1").arg(curr_num));
            pcmd->setFocusPolicy(Qt::NoFocus);
            pcmd->setMinimumSize(conf->but_cab_type_size); pcmd->setMaximumSize(conf->but_cab_type_size);
            pcmd->move(conf->but_cab_type_H_offset[v_pos], conf->but_V_offset * v_slot[v_pos]);
            pcmd->setVisible(true);
//            QObject::connect(pcmd, SIGNAL(clicked(bool)), this, SLOT(but_cab_clicked()));

            pcmd = new QPushButton("↑↑", this);
//            pcmd->setObjectName(QString("but_c_up_%1").arg(curr_num));
            pcmd->setObjectName(QString("but_c_up_%1_%2").arg(curr_num).arg(v_pos));
            pcmd->setFocusPolicy(Qt::NoFocus);
            pcmd->setMinimumSize(conf->but_cab_move_size); pcmd->setMaximumSize(conf->but_cab_move_size);
            pcmd->move(conf->but_cab_up_H_offset[v_pos], conf->but_V_offset * v_slot[v_pos]);
            pcmd->setVisible(true);
            QObject::connect(pcmd, SIGNAL(clicked(bool)), this, SLOT(but_cab_move_clicked()));

            pcmd = new QPushButton("↔", this);
            pcmd->setObjectName(QString("but_c_lr_%1_%2").arg(curr_num).arg(v_pos));
            pcmd->setFocusPolicy(Qt::NoFocus);
            pcmd->setMinimumSize(conf->but_cab_move_size); pcmd->setMaximumSize(conf->but_cab_move_size);
            pcmd->move(conf->but_cab_lr_H_offset[v_pos], conf->but_V_offset * v_slot[v_pos]);
            pcmd->setVisible(true);
            QObject::connect(pcmd, SIGNAL(clicked(bool)), this, SLOT(but_cab_move_clicked()));

            for (int row = 0; row < coup_pn.length(); ++row) {
                v_slot[v_pos]++;
                but_xy = QPoint(conf->but_fiber_H_offset[v_pos], conf->but_V_offset * v_slot[v_pos]);
                p_id = coup_pn.at(row)["id"].toInt();
                ports.insert(p_id, but_xy);

                pcmd = new QPushButton(QString("%1").arg(coup_pn.at(row)["fiber_num"].toInt()), this);
                if (! coup_pn.at(row)["p_valid"].toBool())
                    pcmd->setStyleSheet(QString("background-color: %1").arg("gray"));
                pcmd->setObjectName(QString("but_p_%1").arg(p_id));
                pcmd->setFocusPolicy(Qt::NoFocus);
                pcmd->setMinimumSize(conf->but_fiber_size); pcmd->setMaximumSize(conf->but_fiber_size);
                pcmd->move(but_xy);
                pcmd->setVisible(true);
                pcmd->setCheckable(true);
//                pcmd->setStyleSheet("border-radius: 9px; border-width: 1px; border-color: black; border-style: outset; background-color: beige;");
                QObject::connect(pcmd, SIGNAL(clicked(bool)), this, SLOT(but_fiber_clicked()));

                lab_xy = QPoint(conf->lab_color_fiber_H_offset[v_pos], conf->but_V_offset * v_slot[v_pos] + 1);
                lbl_f = new QLabel(conf->ru_color_list.value(coup_pn.at(row)["fiber_color"].toString()), this);
                lbl_f->setObjectName(QString("lab_p_%1").arg(p_id));
                lbl_f->setFixedSize(conf->lab_color_fiber_size);
                lbl_f->setAlignment(Qt::AlignHCenter);
                if (f_color) lbl_f->setStyleSheet(QString("background-color: %1").arg(coup_pn.at(row)["fiber_color"].toString()));
                else         lbl_f->setStyleSheet(QString("background-color: beige"));
                lbl_f->move(lab_xy);
                lbl_f->setVisible(true);

                if (mod_s != coup_pn.at(row)["mod_num"].toInt()) {
                    mod_s = coup_pn.at(row)["mod_num"].toInt();
                    lab_xy = QPoint(conf->lab_color_mod_H_offset[v_pos], conf->but_V_offset * v_slot[v_pos] + 1);
                    if (f_color) lbl_m = new QLabel(QString("%1").arg(coup_pn.at(row)["mod_num"].toInt()), this);
                    else         lbl_m = new QLabel(conf->ru_color_list.value(coup_pn.at(row)["mod_color"].toString()).left(1), this);
                    lbl_m->setObjectName(QString("lab_m_%1").arg(p_id));
                    lbl_m->setFixedSize(conf->lab_color_mod_size);
                    lbl_m->setAlignment(Qt::AlignHCenter);
                    if (f_color) lbl_m->setStyleSheet(QString("background-color: %1").arg(coup_pn.at(row)["mod_color"].toString()));
                    else         lbl_m->setStyleSheet(QString("background-color: beige"));
                    lbl_m->move(lab_xy);
                    lbl_m->setVisible(true);
                }
                else if (lbl_m != nullptr) {
                    lbl_m->setFixedHeight(lbl_m->height() + conf->but_V_offset);
                }

                if (coup_pn.at(row)["int_c_dest"].toInt() == 1) {
                    int int_c_id = coup_pn.at(row)["int_c_id"].toInt();
                    cross_xy = QPoint(conf->but_cross_port_H_offset[v_pos], conf->but_V_offset * v_slot[v_pos]);

                    pcmd = new QPushButton(QString("%1").arg(cross_p.value(int_c_id).first), this);
                    pcmd->setObjectName(QString("but_cr_%1").arg(int_c_id));
                    pcmd->setFocusPolicy(Qt::NoFocus);
                    pcmd->setMinimumSize(conf->but_fiber_size); pcmd->setMaximumSize(conf->but_fiber_size);
                    pcmd->move(cross_xy);
                    pcmd->setVisible(true);
//                    pcmd->setStyleSheet(QString("background-color: %1").arg("#eff0f1"));
//                    QObject::connect(pcmd, SIGNAL(clicked(bool)), this, SLOT(but_cab_clicked()));
                    lab_xy = QPoint(conf->lab_cross_H_offset[v_pos], conf->but_V_offset * v_slot[v_pos] + 1);

                    lbl_cr = new QLabel(cross_p.value(int_c_id).second, this);
                    lbl_cr->setObjectName("lab_p_cross");
                    lbl_cr->setFixedSize(conf->lab_cross_size);
                    lbl_cr->setAlignment(Qt::AlignHCenter);
//                    lbl_cr->setStyleSheet(QString("background-color: %1").arg("#eff0f1"));
                    lbl_cr->move(lab_xy);
                    lbl_cr->setVisible(true);
                }
            }
        }
    }
    int fr_size = std::max(v_slot[0],v_slot[1]) * conf->but_V_offset + conf->but_V_offset * 3;
    this->setFixedHeight(fr_size);
//    qDebug() << "fr_size" << fr_size;
}

void CoupPaint::createLinks()
{
    links.clear();
    links_pen.clear();
    links_sel.clear();
    circle0.clear();
    circle1.clear();

    QColor link_col;
    Qt::PenStyle link_line;
    QMap<int, QPoint> ports2 = QMap(ports);
    short v_pos, v_pos2;
    QJsonObject coup_p;
    QPolygon polygon;
    QPoint b_cntr = QPoint(conf->but_fiber_size.width()/2, conf->but_fiber_size.height()/2);
    int dst_id;
    QPoint dst_crd, pnt2, pnt3;
    QRect pnt2c, pnt3c;
    QList<int> v_slot = {0,0};

    QMap<int, QPoint>::iterator curr_p = ports2.begin();
    while (curr_p != ports2.end()) {
        if (curr_p.value().x() == conf->but_fiber_H_offset[0]) v_pos = 0;
        else                                                   v_pos = 1;

        for (auto &row : coup_ports) {
            if (row["id"].toInt() == curr_p.key())
                coup_p = row;
        }
        if (coup_p["int_c_id"].toInt() == 0) {
            polygon << curr_p.value() + b_cntr
                    << (curr_p.value() + QPoint(conf->link_0_offset[v_pos], 0)) + b_cntr ;
            links << polygon;
            polygon.clear();
            circle0 << QRect(curr_p.value() + QPoint(conf->link_0_offset[v_pos], 0) + QPoint(8,8), QSize(4,4));
        } else
            if (coup_p["int_c_dest"].toInt() == 1) {
                polygon << curr_p.value() + b_cntr
                        << curr_p.value() + b_cntr + QPoint(conf->link_3_offset[v_pos], 0);
                links << polygon;
                polygon.clear();
            } else {
                dst_id = coup_p["int_c_id"].toInt();
                dst_crd = ports2.find(dst_id).value();
                if (dst_crd.x() == conf->but_fiber_H_offset[0]) v_pos2 = 0;
                else                                            v_pos2 = 1;
                if (v_pos ^ v_pos2) {
                    pnt2 = curr_p.value() + b_cntr + QPoint(conf->link_1_offset[v_pos], 0) ;
                    pnt3 = dst_crd + b_cntr + QPoint(conf->link_1_offset[v_pos2], 0) ;
                    pnt2c = QRect(pnt2 - QPoint(1,1), QSize(3,3));
                    pnt3c = QRect(pnt3 - QPoint(1,1), QSize(3,3));
                }
                else {
                    pnt2 = curr_p.value() + b_cntr + QPoint(conf->link_2_offset[v_pos] + v_slot[v_pos], 0) ;
                    pnt3 = dst_crd + b_cntr + QPoint(conf->link_2_offset[v_pos2] + v_slot[v_pos], 0) ;
                    pnt2c = QRect(curr_p.value() + b_cntr + QPoint(conf->link_0_offset[v_pos], 0) - QPoint(1,1), QSize(3,3));
                    pnt3c = QRect(dst_crd + b_cntr + QPoint(conf->link_0_offset[v_pos2], 0) - QPoint(1,1), QSize(3,3));
                    if (v_pos2) v_slot[v_pos] -= conf->link_v_slot_step;
                    else        v_slot[v_pos] += conf->link_v_slot_step;
                }
                polygon << curr_p.value() + b_cntr;
                polygon << pnt2 << pnt3;
                polygon << dst_crd + b_cntr ;
                links << polygon;
                polygon.clear();
                ports2.erase(ports2.find(dst_id));

                if (coup_p["int_c_status"].toInt() == 2)
                    circle1 << pnt2c << pnt3c;
            }
        curr_p = ports2.erase(curr_p);

        if (coup_p["int_c_status"].toInt() == 1) link_col = Qt::blue;
        else                                     link_col = Qt::black;
        if (coup_p["changed"].toBool() == 1) link_line = Qt::DashLine;
        else                                 link_line = Qt::SolidLine;

        links_pen << QPen(link_col, 1, link_line/*, Qt::RoundCap, Qt::RoundJoin*/);
    }
}

void CoupPaint::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(Qt::white);
//    QRect rect1;
    back_list.setSize(QSize(549, this->height()-40));
    back_list.moveTo(360, 30);
    painter.drawRect(back_list);

    painter.setBrush(Qt::NoBrush);

    for (int i=0; i<links.count(); i++) {
        painter.setPen(links_pen.at(i));
        painter.drawPolyline(links.at(i));
    }
    painter.setPen(QPen(Qt::red, 3, Qt::SolidLine, Qt::FlatCap));
    for (QPolygon &plg : links_sel) {
        painter.drawPolyline(plg);
    }
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    for (QRect &cic : circle0) {
        painter.drawEllipse(cic);
    }
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
    for (QRect &cic : circle1) {
        painter.drawEllipse(cic);
    }
}

void CoupPaint::but_ext_coup_clicked()
{
    QPushButton *bext = static_cast<QPushButton*>(sender());
    int coup_id = bext->objectName().split("_").at(4).toInt();
    for (auto &ob : cab_links) {
        if (ob["id"].toInt() == coup_id) {
            emit sigNextCoup(coup_id, {ob["coord_x"].toInt(), ob["coord_y"].toInt()});
            break;
        }
    }
//qDebug() << "but_ext_coup_clicked:" << bext->objectName() << "coup_id:" << coup_id;
}

void CoupPaint::but_cab_move_clicked()
{
    QPushButton *bcab = static_cast<QPushButton*>(sender());
//qDebug() << "but_cab_clicked: " << bcab->objectName() << bcab->pos();
    QStringList but_name = bcab->objectName().split("_");
    int cab_n = but_name.at(3).toInt();
    int but_pos = but_name.at(4).toUInt();
    auto &cab_slot_this = (but_pos) ? cab_slot_R : cab_slot_L;
    auto &cab_slot_to = (!but_pos) ? cab_slot_R : cab_slot_L;
    int idx = -1;
    for (int i = 0; i < cab_slot_this.size(); ++i) {
        if (cab_slot_this.at(i)["cable_num"].toInt() == cab_n) {
            idx = i;
            break;
        }
    }
    if (but_name.at(2) == "up" && idx <= 0) return;
    if (but_name.at(2) == "up" && idx > 0) {
        cab_slot_this.swapItemsAt(idx, idx-1);
    } else
    if (but_name.at(2) == "lr") {
        cab_slot_to.append(cab_slot_this.at(idx));
        cab_slot_this.removeAt(idx);
    } else
        return;

    for (QObject* pb : this->children()) {
        if (pb->objectName().startsWith("but_") || pb->objectName().startsWith("lab_"))
            pb->deleteLater();
    }
    postCabList();
    createBut();
    createLinks();
    this->update();
}

void CoupPaint::but_fiber_clicked()
{
    QPushButton *bp = static_cast<QPushButton*>(sender());
//qDebug() << "but_fiber_clicked:" << bp->objectName() << bp->pos() << bp->isChecked();
    QPoint b_cntr = QPoint(conf->but_fiber_size.width()/2, conf->but_fiber_size.height()/2);
    QPoint curr_pos = bp->pos() + b_cntr;

    if (bp->isChecked()){
        for (QPolygon plg : links) {
            if (plg.first() == curr_pos) {
                links_sel << plg;
                QPoint bp2_pos = plg.last() - b_cntr;
                if (ports.key(bp2_pos)) this->findChild< QPushButton * >(QString("but_p_%1").arg(ports.key(bp2_pos)))->setChecked(1);
                break;
            }
            if (plg.last() == curr_pos) {
                links_sel << plg;
                QPoint bp2_pos = plg.first() - b_cntr;
                if (ports.key(bp2_pos)) this->findChild< QPushButton * >(QString("but_p_%1").arg(ports.key(bp2_pos)))->setChecked(1);
                break;
            }
        }
    } else {
        for (int ind = 0; ind < links_sel.length(); ind++) {
            if (links_sel[ind].first() == curr_pos) {
                QPoint bp2_pos = links_sel[ind].last() - b_cntr;
                if (ports.key(bp2_pos)) this->findChild< QPushButton * >(QString("but_p_%1").arg(ports.key(bp2_pos)))->setChecked(0);
                links_sel.removeAt(ind);
                break;
            }
            if (links_sel[ind].last() == curr_pos) {
                QPoint bp2_pos = links_sel[ind].first() - b_cntr;
                if (ports.key(bp2_pos)) this->findChild< QPushButton * >(QString("but_p_%1").arg(ports.key(bp2_pos)))->setChecked(0);
                links_sel.removeAt(ind);
                break;
            }
        }
    }
    this->update();
}
