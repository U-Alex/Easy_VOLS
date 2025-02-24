#include "couppaintext.h"
#include "ui_couppaintext.h"

#include <QPushButton>
#include <QLabel>

CoupPaintExt::CoupPaintExt(Config *ref_conf, uint c_id, short _fr_pos, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CoupPaintExt),
    conf(ref_conf),
    coup_id(c_id),
    fr_pos(_fr_pos)
{
    ui->setupUi(this);
}

CoupPaintExt::~CoupPaintExt()
{
    delete ui;
}

void CoupPaintExt::slotCoupPaintExt(uint c_id, short _fr_pos, QJsonDocument json)
{
    if (coup_id == c_id && fr_pos == _fr_pos) coup_id = 0;
    else return;
//qDebug()<<"slotCoupPaintExt"<<c_id<<fr_pos/*<<json.object()*/;

    int             obj_id = 0, v_slot = -1;
    QString         obj_cr, end_type;
    QStringList     up_info, color_cr, obj_par;
    QPushButton    *pcmd;
    QLabel         *lbl_cr;

    for (auto i = 0; !json[i].isUndefined() ; ++i) {
        auto coup_pn = json[i];
//qDebug() <<coup_pn;
        v_slot += 2;
        up_info = coup_pn[0]["up_info"].toString().split("∿");
        if (up_info.count() < 5) up_info = {"","0","?","?","?"};
        lbl_cr = new QLabel(QString("длина: %1  запас: %2").arg(up_info.at(2)).arg(up_info.at(3)), this);
        lbl_cr->setObjectName(QString("lab_up_info_%1").arg(coup_pn[0]["cable_num"].toInt()));
        lbl_cr->setFixedSize(conf->lab_up_info_size);
        lbl_cr->setAlignment(Qt::AlignHCenter);
//        lbl_cr->setStyleSheet(QString("background-color: %1").arg("silver"));
        lbl_cr->move(conf->lab_up_info_H_offset[fr_pos], conf->but_V_offset * v_slot + 1);
        lbl_cr->setVisible(true);
// qDebug() << lbl_cr->objectName();
        ++v_slot;

        lbl_cr = new QLabel((up_info.at(4) != "None") ? QString("дата ввода: %1").arg(up_info.at(4)) : "дата ввода: ?", this);
        lbl_cr->setObjectName(QString("lab_up_info_%1").arg(coup_pn[0]["cable_num"].toInt()));
        //lbl_cr->setToolTip("");
        lbl_cr->setFixedSize(conf->lab_up_info_size);
        lbl_cr->setAlignment(Qt::AlignHCenter);
//        lbl_cr->setStyleSheet(QString("background-color: %1").arg("silver"));
        lbl_cr->move(conf->lab_up_info_H_offset[fr_pos], conf->but_V_offset * v_slot + 1);
        lbl_cr->setVisible(true);

        for (auto i = 0; !coup_pn[i].isUndefined() ; ++i) {
            auto pn = coup_pn[i];
            ++v_slot;
            end_type = pn["end_type"].toString();
            if (end_type == "coup") {
                obj_id = pn["hop_coup"]["id"].toInt();
                obj_cr = QString("м: %1").arg(pn["hop_coup"]["name"].toString());
                color_cr = {conf->color_coup_ext[pn["hop_coup"]["parr_type"].toInt()], ""};
                obj_par = {pn["hop_coup"]["hop_parent_lo"].toString(), pn["hop_coup"]["hop_parent_full"].toString()};
            }
            if (end_type == "cross") {
                obj_id = pn["hop_cross"]["id"].toInt();
                obj_cr = QString("кр: %1 п: %2").arg(pn["hop_cross"]["name"].toString()).arg(pn["hop_port"]["num"].toInt());
                color_cr = {conf->color_cross[pn["hop_port"]["up_status"].toInt()], conf->color_cross[pn["hop_port"]["int_c_status"].toInt()]};
                obj_par = {pn["hop_cross"]["hop_parent_lo"].toString(), pn["hop_cross"]["hop_parent_full"].toString()};
            }
            if (end_type == "loopback detected") {
                obj_id = 0;
                obj_cr = "loopback detected";
                color_cr = {"red", "red"};
                obj_par = {"",""};
            }
//qDebug() <<end_type<<obj_id<<obj_cr<<color_cr<<obj_par[1];
            pcmd = new QPushButton(QString("%1").arg(pn["count_hop"].toInt()), this);
            pcmd->setObjectName(QString("but_hop_%1").arg(pn["id"].toInt()));
            pcmd->setFocusPolicy(Qt::NoFocus);
            pcmd->setMinimumSize(conf->but_hop_size); pcmd->setMaximumSize(conf->but_hop_size);
            pcmd->move(conf->but_hop_H_offset[fr_pos], conf->but_V_offset * v_slot);
            pcmd->setVisible(true);
            QObject::connect(pcmd, SIGNAL(clicked(bool)), this, SLOT(but_hop_clicked()));

            pcmd = new QPushButton(obj_cr, this);
            pcmd->setObjectName(QString("but_parr_%1_%2").arg(end_type).arg(obj_id));
            pcmd->setToolTip(obj_par[1]);
            pcmd->setFocusPolicy(Qt::NoFocus);
            pcmd->setMinimumSize(conf->but_parrent_size); pcmd->setMaximumSize(conf->but_parrent_size);
            pcmd->move(conf->but_parrent_H_offset[fr_pos], conf->but_V_offset * v_slot);
            pcmd->setVisible(true);
            QObject::connect(pcmd, SIGNAL(clicked(bool)), this, SLOT(but_parr_clicked()));

            if (color_cr[0] != "None") {
                lbl_cr = new QLabel("", this);
                lbl_cr->setObjectName(QString("lab_cr1_%1").arg(obj_id));
                lbl_cr->setToolTip("тип внешней связи");
                lbl_cr->setFixedSize(conf->lab_color_cr_size);
                lbl_cr->setAlignment(Qt::AlignHCenter);
                lbl_cr->setStyleSheet(QString("background-color: %1").arg(color_cr[0]));
                lbl_cr->move(conf->lab_color_cr1_H_offset[fr_pos], conf->but_V_offset * v_slot + 1);
                lbl_cr->setVisible(true);
            }
            if (end_type == "cross" && color_cr[1] != "None") {
                lbl_cr = new QLabel("", this);
                lbl_cr->setObjectName(QString("lab_cr2_%1").arg(obj_id));
                lbl_cr->setToolTip("тип внутренней связи\nв конце цепочки");
                lbl_cr->setFixedSize(conf->lab_color_cr_size);
                lbl_cr->setAlignment(Qt::AlignHCenter);
                lbl_cr->setStyleSheet(QString("background-color: %1").arg(color_cr[1]));
                lbl_cr->move(conf->lab_color_cr2_H_offset[fr_pos], conf->but_V_offset * v_slot + 1);
                lbl_cr->setVisible(true);
            }
        }
    }
}

void CoupPaintExt::but_hop_clicked()
{
    QString c_p_id = sender()->objectName().split("_")[2];
    emit sigShowHopExt(c_p_id.toUInt());
//qDebug() << "but_hop_clicked: emit showHop" << c_p_id;
}

void CoupPaintExt::but_parr_clicked()
{
    QPushButton *bparr = static_cast<QPushButton*>(sender());
qDebug() << "on_bhop_clicked: " << bparr->objectName() << bparr->pos() << bparr->isChecked();
}
