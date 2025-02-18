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
    void getData(ObjType, uint id = 0);
    void setData(ObjType, QMap<int, QList<QVariant>>);
    void getCoupLinks(uint id);
    void createLink(/*uint id, */QMap<QString, QVariant>);
    void deleteLink(uint id);
    void getDataPaint(uint id);
    void getDataPaintExt(uint id, QStringList cab_list, short fr_pos);

signals:
    void sigAuthResult(bool);
    void sigDataToObj(ObjType, uint, QJsonDocument);
    void sigCoupLinks(/*uint, */QJsonDocument/*, bool*/);
    void sigCoupPaint(uint, QJsonDocument);
    void sigCoupPaintExt(uint, short, QJsonDocument);

private:
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
