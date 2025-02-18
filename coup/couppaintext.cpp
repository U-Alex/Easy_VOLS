#include "couppaintext.h"
#include "ui_couppaintext.h"

CoupPaintExt::CoupPaintExt(Config *ref_conf, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CoupPaintExt),
    conf(ref_conf)
{
    ui->setupUi(this);
}

CoupPaintExt::~CoupPaintExt()
{
    delete ui;
}

void CoupPaintExt::slotCoupPaintExt(uint c_id, short fr_pos, QJsonDocument json)
{
qDebug()<<"slotCoupPaintExt"<<c_id<<fr_pos<<json;
}
