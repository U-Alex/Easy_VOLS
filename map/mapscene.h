#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QObject>
#include <QGraphicsScene>

class MapScene : public QGraphicsScene
{
public:
    explicit MapScene(QObject *parent);
    ~MapScene() override;


};

#endif // MAPSCENE_H
