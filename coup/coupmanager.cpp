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

void CoupManager::nextCoup(uint c_id, QPoint pos)
{
    QObject *ob = static_cast<QObject*>(sender());
    if (ob != 0) emit sigToMapCoup(/*c_id, */pos);

    coup_id = c_id;
    if (coupPaint != nullptr) coupPaint->deleteLater();
//    coupPaint = new CoupPaint(conf, /*userSession, */ui->frame_P);
    coupPaint = new CoupPaint(conf, /*userSession, */this);
    ui->gridLayout->addWidget(coupPaint, 0, 1);
    QObject::connect(coupPaint, &CoupPaint::nextCoup, this, &CoupManager::nextCoup);
    QObject::connect(userSession, &UserSession::sigCoupPaint, coupPaint, &CoupPaint::slotCoupPaint);
    userSession->getDataPaint(c_id);
    fr_P_repaint(0);
    fr_P_repaint(1);
}

void CoupManager::fr_P_repaint(short fr_pos)
{
//    if ((fr_pos == 0 || fr_pos == 2) && ui->pb_L->isChecked()) {
//        if (c_ext_L != 0) c_ext_L->deleteLater();
//        c_ext_L = new sh_Coup_ext(conf, orm, coup_id, c_paint->cab_n_L, 0, ui->frame_L);
//        connect(c_ext_L, &sh_Coup_ext::show_hop, this, &sh_Coup::show_hop);
//    }
//    if ((fr_pos == 1 || fr_pos == 2) && ui->pb_R->isChecked()) {
//        if (c_ext_R != 0) c_ext_R->deleteLater();
//        c_ext_R = new sh_Coup_ext(conf, orm, coup_id, c_paint->cab_n_R, 1, ui->frame_R);
//        connect(c_ext_R, &sh_Coup_ext::show_hop, this, &sh_Coup::show_hop);
//    }
//    ui->frame_P->setFixedHeight(this->coupPaint->height());
    ui->scrollAreaWidgetContents->setFixedHeight(this->coupPaint->height() + 10);
//    ui->frame_L->setFixedHeight(this->coupPaint->height());
//    ui->frame_R->setFixedHeight(this->coupPaint->height());
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
//        ui->frame_L->setFixedWidth(frame_LR_width + 20);
        ui->frame_head_L->setFixedWidth(frame_LR_width + 20);
//        ui->frame_P->move(ui->frame_P->pos() + QPoint(frame_LR_width, 0));
        ui->frame_head->move(ui->frame_head->pos() + QPoint(frame_LR_width, 0));
//        ui->frame_R->move(ui->frame_R->pos() + QPoint(frame_LR_width, 0));
        ui->frame_head_R->move(ui->frame_head_R->pos() + QPoint(frame_LR_width, 0));
        ui->scrollArea->setFixedWidth(ui->scrollArea->width() + frame_LR_width);
        this->setFixedWidth(this->width() + frame_LR_width);
        fr_P_repaint(0);
        bp->setText(">>");
        this->move(this->x()-frame_LR_width, this->y());
    } else {
//        ui->frame_L->setFixedWidth(20);
        ui->frame_head_L->setFixedWidth(20);
//        ui->frame_P->move(ui->frame_P->pos() - QPoint(frame_LR_width, 0));
        ui->frame_head->move(ui->frame_head->pos() - QPoint(frame_LR_width, 0));
//        ui->frame_R->move(ui->frame_R->pos() - QPoint(frame_LR_width, 0));
        ui->frame_head_R->move(ui->frame_head_R->pos() - QPoint(frame_LR_width, 0));
        ui->scrollArea->setFixedWidth(ui->scrollArea->width() - frame_LR_width);
        this->setFixedWidth(this->width() - frame_LR_width);
//        c_ext_L->deleteLater();
//        c_ext_L = 0;
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
//        ui->frame_R->setFixedWidth(frame_LR_width + 20);
        ui->frame_head_R->setFixedWidth(frame_LR_width + 20);
        ui->scrollArea->setFixedWidth(ui->scrollArea->width() + frame_LR_width);
        this->setFixedWidth(this->width() + frame_LR_width);
        fr_P_repaint(1);
        bp->setText("<<");
    }
    else {
//        ui->frame_R->setFixedWidth(20);
        ui->frame_head_R->setFixedWidth(20);
        ui->scrollArea->setFixedWidth(ui->scrollArea->width() - frame_LR_width);
        this->setFixedWidth(this->width() - frame_LR_width);
//        c_ext_R->deleteLater();
//        c_ext_R = 0;
        bp->setText(">>");
    }
}

