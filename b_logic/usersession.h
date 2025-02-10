#ifndef USERSESSION_H
#define USERSESSION_H

#include "b_logic/restaccessmanager.h"
#include "map/obj/obj.h"
//#include "map/obj/objfab.h"

class UserSession : public QObject
{
    Q_OBJECT
public:
    explicit UserSession(QObject *parent = nullptr);
    ~UserSession();

    void auth(QString login, QString pass);
    void getData(ObjType objType, uint id = 0);
    void getCoupLinks(uint id);

signals:
    void sigAuthResult(bool);
    void sigDataToObj(ObjType, uint, QJsonDocument);
    void sigCoupLinks(uint, QJsonDocument);

private:
//    ObjFab            *objFab;
    RestAccessManager *_ram;
    struct User {
        int     u_id;
        QString name;
        QString email;
//        QByteArray token;
    };
    std::optional<User> user;

};

#endif // USERSESSION_H
