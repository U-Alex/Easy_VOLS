#include "usersession.h"
#include <QJsonDocument>
#include <QJsonObject>//
#include <QJsonArray>//
#include <QUrlQuery>
//#include <chrono>           //
//#include <thread>           //
//    std::this_thread::sleep_for(std::chrono::seconds(2));

UserSession::UserSession(QObject *parent) :
    QObject(parent)
{
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
            auto token = json["token"].toString("");
            if (!err.error && !token.isEmpty()) {
                _ram->setAuthorizationToken(token.toUtf8());
                emit sigAuthResult(true);
                return;
//            else emit(error...);
            }
        }
        emit sigAuthResult(false);
    };
    QString api{"/api/vols/api-token-auth/"};
    QVariantMap param{};
    param.insert("username", login);
    param.insert("password", pass);
    _ram->post(api, param, callback);
}

//--------------------------------------------------------------------------

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

void UserSession::setData(ObjType objType, QMap<int, QList<QVariant>> data)
{
    qDebug() << "setData:" << (int)objType << data;
    RestAccessManager::ResponseCallback callback = [/*this*/]
            (QNetworkReply* reply, bool success) {
        if (success) {
            QJsonParseError err;
            auto json = QJsonDocument::fromJson(reply->readAll(), &err);
            if (!err.error) {
//                emit sigCoupLinks(/*id, */json/*, false*/);
//            qDebug() << "json:" << json;
            }
//            else emit(error...);
        }
    };
    QString api{"/api/vols/"};
    QJsonArray params;
    QJsonObject param;
    switch(objType) {
        case ObjType::o_pw_cont:
            api.append("pwcont/");
            for (auto [key, value] : data.asKeyValueRange()) {
                param.insert("id", key);
                param.insert("coord_x", value.at(0).toInt());
                param.insert("coord_y", value.at(1).toInt());
                params << param;
            }
            break;
//        case ObjType::o_locker:
//            api.append("locker/"); break;
        case ObjType::o_coup:
            api.append("coup/");
            for (auto [key, value] : data.asKeyValueRange()) {
                param.insert("id", key);
                param.insert("coord_x", value.at(0).toInt());
                param.insert("coord_y", value.at(1).toInt());
                params << param;
            }
            break;
        case ObjType::o_polyline:
            api.append("polyline/");
            for (auto [key, value] : data.asKeyValueRange()) {
                param.insert("id", key);
                param.insert("path", value.at(0).toString());
                param.insert("param", value.at(1).toString());
                params << param;
            }
            break;
        case ObjType::o_label:
            api.append("label/");

            break;
        default:
            return;
    }
//qDebug() << "params:" << params;
    _ram->post(api, QJsonDocument(params), callback);
}

//--------------------------------------------------------------------------

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

void UserSession::createLink(/*uint id, */QMap<QString, QVariant> data)
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
//    qDebug() << "param"<< param;
    _ram->put(api, param, callback);
}

void UserSession::deleteLink(uint id)
{
    RestAccessManager::ResponseCallback callback = [/*this, id*/]
            (QNetworkReply* reply, bool success) {
        if (success) {
            QJsonParseError err;
            auto json = QJsonDocument::fromJson(reply->readAll(), &err);
            if (!err.error) {
//                emit sigDataToObj(ObjType::o_polyline, 0, json);   //id=0
//                emit sigCoupLinks(/*id, */json, true);
            }
//            else emit(error...);
//        qDebug() << "json:" << json;
        }
    };
    QString api = QString("/api/vols/polyline/");
    if (id) api.append(QString("%1/").arg(id));

    _ram->delete_(api, callback);
}

//--------------------------------------------------------------------------




