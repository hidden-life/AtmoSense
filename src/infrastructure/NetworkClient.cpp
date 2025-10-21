#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>

#include "NetworkClient.h"

NetworkClient::NetworkClient() = default;

QJsonDocument NetworkClient::getJson(const QUrl &url, const int timeout) {
    QNetworkRequest request(url);
    request.setRawHeader("Accept", "application/json");

    QNetworkReply *reply = m_manager.get(request);

    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);

    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    timer.start(timeout);
    loop.exec();

    if (timer.remainingTime() == 0) {
        reply->abort();
        reply->deleteLater();

        throw std::runtime_error("Network timeout.");
    }

    if (reply->error() != QNetworkReply::NoError) {
        auto err = reply->errorString();
        reply->deleteLater();

        throw std::runtime_error(QString("Network error: %1").arg(err).toStdString());
    }

    const QByteArray data = reply->readAll();
    reply->deleteLater();

    QJsonParseError parseError = {};
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        throw std::runtime_error(QString("JSON parse error: %1").arg(parseError.errorString()).toStdString());
    }

    return doc;
}
