#include "mapmanagerlink.h"
#include "ui_mapmanagerlink.h"

#include "map/obj/obj.h"

MapManagerLink::MapManagerLink(UserSession *us, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MapManagerLink),
    userSession(us)
{
    ui->setupUi(this);
}

MapManagerLink::~MapManagerLink()
{
    delete ui;
}

void MapManagerLink::setCoup(ObjCoup *ref_coup)
{
    qDebug() << "setCoup" << ref_coup;
    coup = ref_coup;
    //remove old coups...

    userSession->getCoupLinks(coup->data((int)Idx::o_id).toUInt());
}
