#ifndef MAPMANAGERLINK_H
#define MAPMANAGERLINK_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>

#include "config.h"
#include "map/mapscene.h"
#include "b_logic/usersession.h"
#include "map/obj/objcoup.h"

namespace Ui {
class MapManagerLink;
}

class MapManagerLink : public QFrame
{
    Q_OBJECT
public:
    explicit MapManagerLink(Config *ref_conf, UserSession *us, MapScene *scene, QWidget *parent = nullptr);
    ~MapManagerLink();

public slots:
    void setCoup(ObjCoup *);
    void slotCoupLinks(/*uint, */QJsonDocument/*, bool*/);

private:
    Ui::MapManagerLink *ui;
    Config             *conf = nullptr;
    UserSession        *userSession = nullptr;
    MapScene           *scene = nullptr;

    ObjCoup            *coup = nullptr;
    uint                coup_id;
    QVector<QMap<QString, QVariant>>   coup_links;
    QList<QVector<uint>>               current_links;


private slots:
    void createButCabList();
    uint linkExists(uint, uint, QPair<uint, uint>);
    void butVClicked();
    void butXClicked();

};

#endif // MAPMANAGERLINK_H
