#include "couppaint.h"
#include "ui_couppaint.h"

#include <QPushButton>


CoupPaint::CoupPaint(Config *ref_conf, /*UserSession *us, */QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CoupPaint),
    conf(ref_conf)
//    userSession(us)
{
    ui->setupUi(this);
}

CoupPaint::~CoupPaint()
{
    delete ui;
}

void CoupPaint::slotCoupPaint(uint c_id, QJsonDocument json)
{
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
qDebug() << "v_slot"<< v_slot;
    createBut();
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
    QLabel         *lbl_ext, *lbl_f, *lbl_cr, *lbl_m;

    for (short v_pos = 0; v_pos < 2; ++v_pos) {
        if (v_pos == 0) curr_len = cab_slot_L.length();
        else            curr_len = cab_slot_R.length();
qDebug() << "v_slot---" << v_pos;
        for (int ncab = 0; ncab < curr_len; ++ncab) {
            mod_s = 0;
            if (v_pos == 0) {curr_num = cab_slot_L[ncab]["cable_num"].toInt();
                             curr_type = cab_slot_L[ncab]["cab_name"].toString();}
            else            {curr_num = cab_slot_R[ncab]["cable_num"].toInt();
                             curr_type = cab_slot_R[ncab]["cab_name"].toString();}
qDebug() << curr_num << curr_type;
            QList<QJsonObject> coup_pn;
            foreach (QJsonObject row, coup_ports) {
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
            pcmd->setVisible(1);
            pcmd->setStyleSheet(QString("background-color: %1").arg("silver"));
            QObject::connect(pcmd, SIGNAL(clicked(bool)), this, SLOT(but_ext_coup_clicked()));

            lbl_ext = new QLabel(ext_coup_name, this);
            lbl_ext->setObjectName(QString("lab_ext_coup_%1").arg(p_id));
            lbl_ext->setFixedSize(conf->lab_ext_coup_size);
            lbl_ext->setAlignment(Qt::AlignHCenter);
            lbl_ext->setStyleSheet(QString("background-color: %1").arg("#eff0f1"));
            lbl_ext->move(QPoint(conf->lab_ext_coup_H_offset[v_pos], conf->but_V_offset * v_slot[v_pos] + 1));
            lbl_ext->setVisible(1);

            v_slot[v_pos]++;




        }


    }



}

void CoupPaint::createLinks()
{

}



void CoupPaint::but_ext_coup_clicked()
{
    QPushButton *bext = static_cast<QPushButton*>(sender());
    bool ok;
    int coup_id = bext->objectName().mid(15).toUInt(&ok, 10);
    foreach (auto ob, cab_links) {
        if (ob["id"].toInt() == coup_id) {
            emit nextCoup(coup_id, {ob["coord_x"].toInt(), ob["coord_y"].toInt()});
            break;
        }
    }
//qWarning() << "but_ext_coup_clicked:" << bext->objectName() << "coup_id:" << coup_id;
//    emit nextCoup(coup_id);
}
