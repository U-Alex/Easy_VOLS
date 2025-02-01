#include "mapscene.h"

MapScene::MapScene(QObject *parent) :
    QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::BspTreeIndex);//::NoIndex

}

MapScene::~MapScene() {}
