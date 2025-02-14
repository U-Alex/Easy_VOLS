#include "usersession.h"
#include <QJsonDocument>
#include <QUrlQuery>
//#include <chrono>           //
//#include <thread>           //
//    std::this_thread::sleep_for(std::chrono::seconds(2));

UserSession::UserSession(QObject *parent) :
    QObject(parent)
{
    Q_UNUSED(parent)
    _ram = new RestAccessManager(this);
    _ram->setUrl(QUrl("http://192.168.1.11:8002"));

}

UserSession::~UserSession()
{
    _ram->deleteLater();
}

void UserSession::auth(QString login, QString pass)
{
    RestAccessManager::ResponseCallback callback = [this](QNetworkReply* reply, bool success) {
        if (success) {
            QJsonParseError err;
            auto json = QJsonDocument::fromJson(reply->readAll(), &err);
//        qDebug() << "json token:" << json["token"].toString();
        qDebug() << "json :" << json;
            auto token = json["token"].toString("");
            if (!err.error && !token.isEmpty()) {
                _ram->setAuthorizationToken(token.toUtf8());
                emit sigAuthResult(true);
//            else emit(error...);
            }
        }
    };
    QVariantMap param{};
//    QMap<QString, QVariant> _pair;
    param.insert("username", login);
    param.insert("password", pass);
//    param.insert("data", _pair);
    _ram->post("/api/vols/api-token-auth/", param, callback);

}

void UserSession::getData(ObjType objType, uint id)
{
    RestAccessManager::ResponseCallback callback = [this, objType, id]
            (QNetworkReply* reply, bool success) {
        if (success) {
            QJsonParseError err;
            auto json = QJsonDocument::fromJson(reply->readAll(), &err);
            if (!err.error)
                emit sigDataToObj(objType, id, json);
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
        case ObjType::o_polyline:
            api.append("polyline/"); break;
        case ObjType::o_label:
            api.append("label/"); break;
        default:
            return;
    }
    if (id) api.append(QString("%1").arg(id));
    QUrlQuery param("");
    _ram->get(api, param, callback);
}

void UserSession::getCoupLinks(uint id)
{
    RestAccessManager::ResponseCallback callback = [this/*, id*/]
            (QNetworkReply* reply, bool success) {
        if (success) {
            QJsonParseError err;
            auto json = QJsonDocument::fromJson(reply->readAll(), &err);
            if (!err.error)
                emit sigCoupLinks(/*id, */json/*, false*/);
//            else emit(error...);
//        qDebug() << "json:" << json;
        }
    };

    QString api = QString("/api/vols/coup/%1/links/").arg(id);
    QUrlQuery param("");
    _ram->get(api, param, callback);
}

void UserSession::createCoupLink(/*uint id, */QMap<QString, QVariant> data)
{
    RestAccessManager::ResponseCallback callback = [this/*, id*/]
            (QNetworkReply* reply, bool success) {
        if (success) {
            QJsonParseError err;
            auto json = QJsonDocument::fromJson(reply->readAll(), &err);
            if (!err.error) {
                emit sigDataToObj(ObjType::o_polyline, 0, json);   //id=0
//                emit sigCoupLinks(/*id, */json, true);
            }
//            else emit(error...);
//        qDebug() << "json:" << json;
        }
    };

//  lineidid, linecncn, cabtype, cabcolor, path, param

    QString api = QString("/api/vols/polyline/");
//    if (id) api.append(QString("%1").arg(id));
    QVariantMap param{};
    param.insert("lineidid", QString("%1,%2").arg(data.value("coup_id").toInt()).arg(data.value("ext_coup_id").toInt()));
    param.insert("linecncn", QString("%1,%2").arg(data.value("cable_num").toInt()).arg(data.value("ext_cable_num").toInt()));
    param.insert("cabtype", data.value("cable_name").toString());
    param.insert("cabcolor", data.value("cab_color").toString());
    param.insert("path", QString("%1,%2||%3,%4")
                 .arg(data.value("coup_x").toInt()).arg(data.value("coup_y").toInt())
                 .arg(data.value("ext_coup_x").toInt()).arg(data.value("ext_coup_y").toInt()));
    param.insert("param", QString("%1,1,4").arg(data.value("cab_width").toString()));
    qDebug() << "param"<< param;
    _ram->put(api, param, callback);
}






