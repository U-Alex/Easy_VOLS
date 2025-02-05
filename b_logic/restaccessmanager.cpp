#include "restaccessmanager.h"

using namespace Qt::StringLiterals;
static constexpr auto contentTypeJson = "application/json"_L1;
static const auto authorizationToken = "TOKEN"_ba;


RestAccessManager::RestAccessManager(QObject *parent) :
    QNetworkAccessManager(parent)
{ }

static bool httpResponseSuccess(QNetworkReply* reply)
{
    const int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const bool isReplyError = (reply->error() != QNetworkReply::NoError);

    qDebug() << "Request to path" << reply->request().url().path() << "finished, " << "HTTP:" <<  httpStatusCode;
    if (isReplyError) qDebug() << "Error" << reply->error();
//    else          qDebug() << "HTTP:" <<  httpStatusCode;

    return (!isReplyError && (httpStatusCode >= 200 && httpStatusCode < 300));
}

void RestAccessManager::setUrl(const QUrl& url)
{
    m_url = url;
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
//    request.setRawHeader("Authorization", authorizationToken2+m_authorizationToken);
//    qDebug() << "setRawHeader:" << authorizationToken2+m_authorizationToken;
    QNetworkReply* reply = QNetworkAccessManager::get(request);
    QObject::connect(reply, &QNetworkReply::finished, reply, [reply, callback](){ callback(reply, httpResponseSuccess(reply)); });
}

void RestAccessManager::post(const QString& api, const QVariantMap& value, ResponseCallback callback)
{
    m_url.setPath(api);
    auto request = QNetworkRequest(m_url);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, contentTypeJson);
    request.setRawHeader(authorizationToken, m_authorizationToken);
    QNetworkReply* reply = QNetworkAccessManager::post(request, QJsonDocument::fromVariant(value).toJson(QJsonDocument::Compact));
    QObject::connect(reply, &QNetworkReply::finished, reply, [reply, callback](){ callback(reply, httpResponseSuccess(reply)); });
}

void RestAccessManager::put(const QString& api, const QVariantMap& value, ResponseCallback callback)
{
    m_url.setPath(api);
    auto request = QNetworkRequest(m_url);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, contentTypeJson);
    request.setRawHeader(authorizationToken, m_authorizationToken);
    QNetworkReply* reply = QNetworkAccessManager::put(request, QJsonDocument::fromVariant(value).toJson(QJsonDocument::Compact));
    QObject::connect(reply, &QNetworkReply::finished, reply, [reply, callback](){ callback(reply, httpResponseSuccess(reply)); });
}

void RestAccessManager::delete_(const QString& api, ResponseCallback callback)
{
    m_url.setPath(api);
    auto request = QNetworkRequest(m_url);
    request.setRawHeader(authorizationToken, m_authorizationToken);
    QNetworkReply* reply = QNetworkAccessManager::deleteResource(request);
    QObject::connect(reply, &QNetworkReply::finished, reply, [reply, callback](){ callback(reply, httpResponseSuccess(reply)); });
}


