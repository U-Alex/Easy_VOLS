#include "restaccessmanager.h"

#include "b_logic/logger.h"

using namespace Qt::StringLiterals;
static constexpr auto contentTypeJson = "application/json"_L1;
static const auto authorizationToken = "TOKEN"_ba;


RestAccessManager::RestAccessManager(QObject *parent) :
    QNetworkAccessManager(parent)
{ }

static bool httpResponseSuccess(QNetworkReply* reply, QString method)
{
    const int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const bool isReplyError = (reply->error() != QNetworkReply::NoError);
    const bool result = (!isReplyError && (httpStatusCode >= 200 && httpStatusCode < 300));

    Logger::sendLog({QString("Request %1 %2 finished, HTTP: %3").arg(method).arg(reply->request().url().path()).arg(httpStatusCode)},
                    (result) ? (method == "GET" ? "green" : "blue" ) : "red");
    if (isReplyError) Logger::sendLog({QString(reply->errorString())}, "red");

    return result;
}

void RestAccessManager::setUrl(const QUrl& url)
{
    m_url = url;
    Logger::sendLog({"setting server url: ", url.toString()});
}

bool RestAccessManager::sslSupported() const
{
#if QT_CONFIG(ssl)
    return QSslSocket::supportsSsl();
#else
    return false;
#endif
}

void RestAccessManager::setAuthorizationToken(const QByteArray& token)
{
    m_authorizationToken = token;
}

void RestAccessManager::get(const QString& api, const QUrlQuery& parameters, ResponseCallback callback)
{
    m_url.setPath(api);
    m_url.setQuery(parameters);
    auto request = QNetworkRequest(m_url);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, contentTypeJson);
    request.setRawHeader(authorizationToken, m_authorizationToken);
    QNetworkReply* reply = QNetworkAccessManager::get(request);
    QObject::connect(reply, &QNetworkReply::finished, reply, [reply, callback](){ callback(reply, httpResponseSuccess(reply, "GET")); });
}

void RestAccessManager::post(const QString& api, const QVariantMap& value, ResponseCallback callback)
{
    post(api, QJsonDocument::fromVariant(value), callback);
}

void RestAccessManager::post(const QString& api, const QJsonDocument& value, ResponseCallback callback)
{
    m_url.setPath(api);
    auto request = QNetworkRequest(m_url);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, contentTypeJson);
    request.setRawHeader(authorizationToken, m_authorizationToken);
    QNetworkReply* reply = QNetworkAccessManager::post(request, value.toJson(QJsonDocument::Compact));
    QObject::connect(reply, &QNetworkReply::finished, reply, [reply, callback](){ callback(reply, httpResponseSuccess(reply, "POST")); });
}

void RestAccessManager::put(const QString& api, const QVariantMap& value, ResponseCallback callback)
{
    m_url.setPath(api);
    auto request = QNetworkRequest(m_url);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, contentTypeJson);
    request.setRawHeader(authorizationToken, m_authorizationToken);
    QNetworkReply* reply = QNetworkAccessManager::put(request, QJsonDocument::fromVariant(value).toJson(QJsonDocument::Compact));
    QObject::connect(reply, &QNetworkReply::finished, reply, [reply, callback](){ callback(reply, httpResponseSuccess(reply, "PUT")); });
}

void RestAccessManager::delete_(const QString& api, ResponseCallback callback)
{
    m_url.setPath(api);
    auto request = QNetworkRequest(m_url);
    request.setRawHeader(authorizationToken, m_authorizationToken);
    QNetworkReply* reply = QNetworkAccessManager::deleteResource(request);
    QObject::connect(reply, &QNetworkReply::finished, reply, [reply, callback](){ callback(reply, httpResponseSuccess(reply, "DELETE")); });
}


