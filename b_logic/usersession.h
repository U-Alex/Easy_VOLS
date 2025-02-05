#ifndef USERSESSION_H
#define USERSESSION_H

#include "b_logic/restaccessmanager.h"
#include "map/obj/objfab.h"


enum class ObjType {
    o_pw_cont, o_locker, o_coup
};

class UserSession : public QObject
{
    Q_OBJECT
public:
    explicit UserSession(QObject *parent = nullptr);
    ~UserSession();

    void auth(QString login, QString pass);
    void getObj(ObjType objType, uint id = 0);

signals:
    void authResult(bool);
    void objResult(ObjType, uint, QList<QGraphicsObject*>);

private slots:
    void recieveObj(ObjType, uint, QJsonDocument);

private:
    ObjFab            *objFab;
//    static ObjType objType;
    RestAccessManager *_ram;
    struct User {
        int     id;
        QString name;
        QString email;
//        QByteArray token;
    };
    std::optional<User> user;

};

#endif // USERSESSION_H
