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
