#include "coupmanager.h"
#include "ui_coupmanager.h"

CoupManager::CoupManager(Config *ref_conf, UserSession *us, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoupManager),
    conf(ref_conf),
    userSession(us)
{
    ui->setupUi(this);
//    nextCoup(1);   //
}

CoupManager::~CoupManager()
{
    delete ui;
}

void CoupManager::slotNextCoup(uint c_id, QPoint pos)
{
    nextCoup(c_id);
    emit sigToMapCoup(pos);
}

void CoupManager::slotNextCoupExt(QStringList c_l, QStringList c_r)
{
    cab_L = c_l; cab_R = c_r;
    fr_P_repaint(2);
}

void CoupManager::nextCoup(uint c_id)
{
    coup_id = c_id;
    if (coupPaint != nullptr) coupPaint->deleteLater();
    coupPaint = new CoupPaint(conf, coup_id, this);
    ui->gridLayout->addWidget(coupPaint, 0, 1);
    QObject::connect(coupPaint, &CoupPaint::sigNextCoup, this, &CoupManager::slotNextCoup);
    QObject::connect(coupPaint, &CoupPaint::sigNextCoupExt, this, &CoupManager::slotNextCoupExt);
    QObject::connect(userSession, &UserSession::sigCoupPaint, coupPaint, &CoupPaint::slotCoupPaint);
//    userSession->getDataPaint(1);//
    userSession->getDataPaint(c_id);
//    userSession->getDataPaint(1);//
//    userSession->getDataPaint(c_id);//
//    fr_P_repaint(2);
}

void CoupManager::fr_P_repaint(short fr_pos)
{
    if ((fr_pos == 0 || fr_pos == 2) && ui->pb_L->isChecked()) {
        if (coupPaintExtL != nullptr) coupPaintExtL->deleteLater();
//        coupPaintExtL = new CoupPaintExtL(conf, orm, coup_id, c_paint->cab_n_L, 0, ui->frame_L);
        coupPaintExtL = new CoupPaintExt(conf, coup_id, 0, this);
        ui->gridLayout->addWidget(coupPaintExtL, 0, 0);
//        QVector<int> cab_list = coupPaint->getCabList(0);
        QObject::connect(userSession, &UserSession::sigCoupPaintExt, coupPaintExtL, &CoupPaintExt::slotCoupPaintExt);
        userSession->getDataPaintExt(coup_id, cab_L, 0);
//        connect(c_ext_L, &sh_Coup_ext::show_hop, this, &sh_Coup::show_hop);
    }
    if ((fr_pos == 1 || fr_pos == 2) && ui->pb_R->isChecked()) {
        if (coupPaintExtR != nullptr) coupPaintExtR->deleteLater();
//        coupPaintExtR = new CoupPaintExtR(conf, orm, coup_id, c_paint->cab_n_R, 1, ui->frame_R);
        coupPaintExtR = new CoupPaintExt(conf, coup_id, 1, this);
        ui->gridLayout->addWidget(coupPaintExtR, 0, 2);
//        QVector<int> cab_list = coupPaint->getCabList(1);
        QObject::connect(userSession, &UserSession::sigCoupPaintExt, coupPaintExtR, &CoupPaintExt::slotCoupPaintExt);
        userSession->getDataPaintExt(coup_id, cab_R, 1);
//        connect(c_ext_R, &sh_Coup_ext::show_hop, this, &sh_Coup::show_hop);
    }
}

void CoupManager::on_pb_L_toggled(bool checked)
{
    QPushButton *bp = static_cast<QPushButton*>(sender());
    if (coupPaint == nullptr) {
        bp->setChecked(0);
        return;
    }
    int frame_LR_width = conf->frame_LR_width;
    if (checked) {
        ui->frame_head_L->setFixedWidth(frame_LR_width + 20);
        ui->frame_head->move(ui->frame_head->pos() + QPoint(frame_LR_width, 0));
        ui->frame_head_R->move(ui->frame_head_R->pos() + QPoint(frame_LR_width, 0));
        ui->scrollArea->setFixedWidth(ui->scrollArea->width() + frame_LR_width);
        this->setFixedWidth(this->width() + frame_LR_width);
        fr_P_repaint(0);
        bp->setText(">>");
        this->move(this->x()-frame_LR_width, this->y());
    } else {
        ui->frame_head_L->setFixedWidth(20);
        ui->frame_head->move(ui->frame_head->pos() - QPoint(frame_LR_width, 0));
        ui->frame_head_R->move(ui->frame_head_R->pos() - QPoint(frame_LR_width, 0));
        ui->scrollArea->setFixedWidth(ui->scrollArea->width() - frame_LR_width);
        this->setFixedWidth(this->width() - frame_LR_width);
        coupPaintExtL->deleteLater();
        coupPaintExtL = nullptr;
        bp->setText("<<");
        this->move(this->x()+frame_LR_width, this->y());
    }
}


void CoupManager::on_pb_R_toggled(bool checked)
{
    QPushButton *bp = static_cast<QPushButton*>(sender());
    if (coupPaint == nullptr) {
        bp->setChecked(0);
        return;
    }
    int frame_LR_width = conf->frame_LR_width;
    if (checked) {
        ui->frame_head_R->setFixedWidth(frame_LR_width + 36);
        ui->scrollArea->setFixedWidth(ui->scrollArea->width() + frame_LR_width);
        this->setFixedWidth(this->width() + frame_LR_width);
        fr_P_repaint(1);
        bp->setText("<<");
    } else {
        ui->frame_head_R->setFixedWidth(36);
        ui->scrollArea->setFixedWidth(ui->scrollArea->width() - frame_LR_width);
        this->setFixedWidth(this->width() - frame_LR_width);
        coupPaintExtR->deleteLater();
        coupPaintExtR = nullptr;
        bp->setText(">>");
    }
}

void CoupManager::on_pb_update_clicked()
{
    if (coup_id) nextCoup(coup_id);
}

