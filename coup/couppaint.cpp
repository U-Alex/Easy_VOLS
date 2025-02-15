#include "couppaint.h"
#include "ui_couppaint.h"

CoupPaint::CoupPaint(Config *ref_conf, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CoupPaint),
    conf(ref_conf)
{
    ui->setupUi(this);
}

CoupPaint::~CoupPaint()
{
    delete ui;
}
