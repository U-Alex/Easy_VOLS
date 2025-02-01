#ifndef USERSESSION_H
#define USERSESSION_H

#include "b_logic/restaccessmanager.h"

class UserSession : public QObject
{
    Q_OBJECT
public:
    explicit UserSession(QObject *parent = nullptr);
//    ~UserSession();

    void auth(QString login, QString pass);
    void getObj(uint id = 0);

signals:
    void authResult(bool);

private:
//    std::shared_ptr<RestAccessManager> ram;
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
