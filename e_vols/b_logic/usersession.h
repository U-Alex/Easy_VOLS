#ifndef USERSESSION_H
#define USERSESSION_H

#include "config.h"
#include "b_logic/restaccessmanager.h"
#include "map/obj/obj.h"

class UserSession : public QObject
{
    Q_OBJECT
public:
    explicit UserSession(Config *ref_conf, QObject *parent = nullptr);
    ~UserSession();

    void auth(QString login, QString pass);
    void getData(ObjType, uint id = 0);
    void setData(ObjType, QMap<int, QList<QVariant>>);
    void getCoupLinks(uint id);
    void createLink(QMap<QString, QVariant>);
    void deleteLink(uint id);
    void getDataPaint(uint id);
    void getDataPaintExt(uint id, QStringList cab_list, short fr_pos);
    void getShowHop(uint c_p_id);

signals:
    void sigAuthResult(bool, QString);
    void sigDataToObj(ObjType, uint, QJsonDocument);
    void sigCoupLinks(QJsonDocument);
    void sigCoupPaint(uint, QJsonDocument);
    void sigCoupPaintExt(uint, short, QJsonDocument);
    void sigShowHopData(QJsonDocument);

private:
    Config            *conf = nullptr;
    RestAccessManager *_ram;

    struct User {
        int     u_id;
        QString name;
        QString email;
    };
    std::optional<User> user;

};

#endif // USERSESSION_H
