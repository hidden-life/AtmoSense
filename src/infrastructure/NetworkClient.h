#ifndef INFRASTRUCTURE_NETWORK_CLIENT_H
#define INFRASTRUCTURE_NETWORK_CLIENT_H

#include <QJsonDocument>
#include <QNetworkAccessManager>

class NetworkClient {
    QNetworkAccessManager m_manager;
public:
    NetworkClient();
    QJsonDocument getJson(const QUrl &url, int timeout = 10000);
};

#endif //INFRASTRUCTURE_NETWORK_CLIENT_H
