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
qDebug()<<"slotCoupPaintExt"<<c_id<<fr_pos/*<<json.object()*/;
//    auto js = json.object();
//    for (auto rec : js) {
//        qDebug()<< rec <<"\n";
//    }

    int             v_slot = -1;
    QStringList     up_info;
    QPushButton    *pcmd;
    QLabel         *lbl_cr;

    for (auto i = 0; !json[i].isUndefined() ; ++i) {
        auto coup_pn = json[i];

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





    }

}
