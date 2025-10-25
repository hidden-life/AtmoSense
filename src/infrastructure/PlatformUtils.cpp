#include "PlatformUtils.h"

#include <QEventLoop>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>

#include "Logger.h"

QString PlatformUtils::osName() {
#ifdef Q_OS_WIN
    return "Windows";
#elif defined(Q_OS_MACOS)
    return "MacOS";
#elif defined(Q_OS_LINUX)
    return "Linux";
#else
    return "Unknown OS";
#endif
}

std::optional<LocationInformation> PlatformUtils::detectLocation() {
    QNetworkAccessManager manager;
    QNetworkRequest req(QUrl("https://ipwho.is/"));
    req.setHeader(QNetworkRequest::UserAgentHeader, "AtmoSenseApplication/1.0");
    QEventLoop loop;

    std::optional<LocationInformation> result;
    auto *reply = manager.get(req);
    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        if (reply->error() == QNetworkReply::NoError) {
            const auto json = QJsonDocument::fromJson(reply->readAll()).object();
            LocationInformation info;
            info.country = json.value("country").toString();
            info.region = json.value("region").toString();
            info.city = json.value("city").toString();
            info.latitude = json.value("latitude").toDouble();
            info.longitude = json.value("longitude").toDouble();
            if (info.isValid()) {
                result = info;
            }
        } else {
            Logger::warning(QString("Location request failed: %1").arg(reply->errorString()));
        }

        loop.quit();
    });

    loop.exec();
    reply->deleteLater();

    return result;
}

void PlatformUtils::setupAutostart(bool enabled) {}
