#ifndef RESTACCESSMANAGER_H
#define RESTACCESSMANAGER_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QJsonDocument>

class RestAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
    Q_PROPERTY(bool sslSupported READ sslSupported CONSTANT)
public:
    explicit RestAccessManager(QObject *parent);

    void setUrl(const QUrl& url);
    void setAuthorizationToken(const QByteArray& token);

    bool sslSupported() const;

    using ResponseCallback = std::function<void(QNetworkReply*, bool)>;
    void post(const QString& api, const QVariantMap& value, ResponseCallback callback);
    void post(const QString& api, const QJsonDocument& value, ResponseCallback callback);
    void get(const QString& api, const QUrlQuery& parameters, ResponseCallback callback);
    void put(const QString& api, const QVariantMap& value, ResponseCallback callback);
    void delete_(const QString& api, ResponseCallback callback);

private:
    QUrl m_url;
    QByteArray m_authorizationToken;
};

#endif // RESTACCESSMANAGER_H
