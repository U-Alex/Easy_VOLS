#include "usersession.h"
#include <QJsonDocument>
#include <QUrlQuery>
//#include <chrono>           //
//#include <thread>           //
//    std::this_thread::sleep_for(std::chrono::seconds(2));

UserSession::UserSession(QObject *parent)
{
    Q_UNUSED(parent)
    _ram = new RestAccessManager(this);
    _ram->setUrl(QUrl("http://192.168.1.11:8002"));
    objFab = new ObjFab();
}

UserSession::~UserSession()
{}

void UserSession::auth(QString login, QString pass)
{
    RestAccessManager::ResponseCallback callback = [this](QNetworkReply* reply, bool success) {
        if (success) {
            QJsonParseError err;
            auto json = QJsonDocument::fromJson(reply->readAll(), &err);
        qDebug() << "json token:" << json["token"].toString();
            auto token = json["token"].toString("");
            if (!err.error && !token.isEmpty()) {
                _ram->setAuthorizationToken(token.toUtf8());
                emit(authResult(true));
//                return;
            }
        }
//        emit(authResult(true));//
    };
    QVariantMap param{};
    QMap<QString, QVariant> _pair;
    _pair.insert("username", login);
    _pair.insert("password", pass);
    param.insert(_pair);
    _ram->post("/api/vols/api-token-auth/", param, callback);

}

void UserSession::getData(ObjType objType, uint id){
//    qDebug() << "start getObj " << id;
    RestAccessManager::ResponseCallback callback = [this, objType, id]
            (QNetworkReply* reply, bool success) {
        if (success) {
            QJsonParseError err;
            auto json = QJsonDocument::fromJson(reply->readAll(), &err);
            if (!err.error)
                recieveObj(objType, id, json);
//                emit(objResult(objType, id, json));
//            else emit(error...);
//        qDebug() << "json:" << json;
        }
    };

    QString api{"/api/vols/"};
    switch(objType) {
        case ObjType::o_pw_cont:
            api.append("pwcont/"); break;
        case ObjType::o_locker:
            api.append("locker/"); break;
        case ObjType::o_coup:
            api.append("coup/"); break;
    }
    if (id) api.append(QString("%1").arg(id));
//    qDebug() << "api:" << api;
    QUrlQuery param("");

    _ram->get(api, param, callback);

}

void UserSession::recieveObj(ObjType objType, uint id, QJsonDocument json)
{
    QList<QGraphicsObject*> list;
    if (!id) {
        switch(objType) {
            case ObjType::o_pw_cont:
                break;
            case ObjType::o_locker:
                list = objFab->createLocker(json);
                break;
            case ObjType::o_coup:
                list = objFab->createCoup(json);
                break;
        }
        emit(objResult(objType, id, list));
    }

}
