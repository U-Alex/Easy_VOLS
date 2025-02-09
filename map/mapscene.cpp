#include "mapscene.h"

#include "map/obj/obj.h"

MapScene::MapScene(QObject *parent) :
    QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::BspTreeIndex);//::NoIndex

}

void MapScene::deselectItems()
{
    foreach (QGraphicsItem *item, this->selectedItems()) {
        item->setSelected(false);
    }
    this->selectedItems().clear();
}

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//qWarning() << this->itemAt(event->scenePos(), QTransform());
    if (event->button() & Qt::LeftButton) {
        QGraphicsItem *item = this->itemAt(event->scenePos(), QTransform());
        QString obj_label = item->data((int)Idx::label).toString();
        if (obj_label == "pwcont")          emit sigPwcontPress(item);
        if (obj_label == "coup")            emit sigCoupPress(item);
        else if (obj_label == "locker")     emit sigLockerPress(item);
        else if (obj_label == "polyline")   emit sigLineClick(item);
    }
    QGraphicsScene::mousePressEvent(event);
}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
//qWarning() << this->itemAt(event->scenePos(), QTransform());
    if (event->button() & Qt::LeftButton) {
        if (selectedItems().length() == 1) {
            QGraphicsItem *item = selectedItems().at(0);
            QString obj_label = item->data((int)Idx::label).toString();
            if (obj_label == "pwcont") {
                emit sigPwcontRelease(item);
            }
            if (obj_label == "coup") {
                //QGraphicsItem *coup = static_cast<QGraphicsItem *>(item);
                emit sigCoupRelease(item);
            }
            else if (obj_label == "label") {
                QGraphicsTextItem *label = static_cast<QGraphicsTextItem *>(item);
                emit sigLabelClick(label);
            }
            //else if (selectedItems().at(0)->data(6).toString() == "polyline") {
                //QGraphicsTextItem *label = static_cast<QGraphicsTextItem *>(selectedItems().at(0));
                //emit sig_line_click(selectedItems().at(0));
            //}
        }
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void MapScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (selectedItems().length() == 0) {
        emit sigBlankClick(event->scenePos().toPoint());
//qWarning() << "emit blank" << event->scenePos().toPoint() ;
    }
    QGraphicsScene::mouseDoubleClickEvent(event);
}

void MapScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Delete: {
        qWarning() << "keyPressEvent" << event->key();
        //foreach (QGraphicsItem *item, selectedItems()) {
        //    removeItem(item);
        //    delete item;
        //}
        //deselectItems();
        break;
    }
    //case Qt::Key_A: {
    //    if(QApplication::keyboardModifiers() & Qt::ControlModifier){
    //        qWarning() << "keyPressEvent" << event->key();
            //foreach (QGraphicsItem *item, items()) {
            //    item->setSelected(true);
            //}
            //if(selectedItems().length() == 1) signalSelectItem(selectedItems().at(0));
    //    }
    //    break;
    //}
    default:
        break;
    }
    QGraphicsScene::keyPressEvent(event);
}

