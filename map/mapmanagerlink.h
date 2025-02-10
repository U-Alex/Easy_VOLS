#ifndef MAPMANAGERLINK_H
#define MAPMANAGERLINK_H

#include <QFrame>

#include "b_logic/usersession.h"
#include "map/obj/objcoup.h"

namespace Ui {
class MapManagerLink;
}

class MapManagerLink : public QFrame
{
    Q_OBJECT
public:
    explicit MapManagerLink(UserSession *us, QWidget *parent = nullptr);
    ~MapManagerLink();

public slots:
    void setCoup(ObjCoup *);

private:
    Ui::MapManagerLink *ui;
    UserSession        *userSession;

    ObjCoup            *coup = nullptr;



};

#endif // MAPMANAGERLINK_H
