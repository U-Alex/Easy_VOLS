#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsItem>
#include <QGraphicsTextItem>

#include <QDebug>

class MapScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MapScene(QObject *parent);
//    ~MapScene() override;

public slots:
    void deselectItems();

signals:
    void sigBlankClick(QPoint pos);
    void sigLabelClick(QGraphicsTextItem *);
    void sigPwcontPress(QGraphicsItem *);
    void sigPwcontRelease(QGraphicsItem *);
    void sigCoupPress(QGraphicsItem *);
    void sigCoupRelease(QGraphicsItem *);
    void sigLockerPress(QGraphicsItem *);
    void sigLineClick(QGraphicsItem *);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

};

#endif // MAPSCENE_H
