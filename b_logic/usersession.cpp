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
    _ram->setUrl(QUrl("https://reqres.in"));
//    auto ram2 = std::make_shared<RestAccessManager>(_ram);
}

//UserSession::~UserSession(){};

void UserSession::auth(QString login, QString pass)
{
    qDebug() << "start auth " << login << pass;
    RestAccessManager::ResponseCallback callback = [this](QNetworkReply* reply, bool success) {
        if (success) {
            QJsonParseError err;
            auto json = QJsonDocument::fromJson(reply->readAll(), &err);
        qDebug() << "json:" << json;
            auto token = json["token"].toString("");
            if (!err.error && !token.isEmpty()) {
                _ram->setAuthorizationToken(token.toUtf8());
                emit(authResult(true));
//                return;
            }
        }
//        emit(authResult(false));
    };
    QVariantMap param{};
    QMap<QString, QVariant> _pair;
//    _pair.insert("email", "peter@klaven");
    _pair.insert("email", "eve.holt@reqres.in");
    _pair.insert("password", "cityslicka");
    param.insert(_pair);
    _ram->post("/api/login", param, callback);
}

void UserSession::getObj(uint id){
    qDebug() << "start getObj " << id;
    RestAccessManager::ResponseCallback callback = [this](QNetworkReply* reply, bool success) {
        if (success) {
            QJsonParseError err;
            auto json = QJsonDocument::fromJson(reply->readAll(), &err);
        qDebug() << "json:" << json;
        }
//        emit(error...);
    };
    QString api{"/api/unknown"};
    if (id) api.append(QString("/%1").arg(id));
    qDebug() << "api:" << api;
    QUrlQuery param("");

    _ram->get(api, param, callback);

}
