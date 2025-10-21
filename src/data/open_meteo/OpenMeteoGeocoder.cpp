#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include  <QUrl>

#include "OpenMeteoGeocoder.h"

#include "Logger.h"

OpenMeteoGeocoder::OpenMeteoGeocoder(NetworkClient &client) : m_client(client) {}

std::vector<Location> OpenMeteoGeocoder::search(const QString &query, const QString &lang, int count) {
    std::vector<Location> result;
    if (query.trimmed().isEmpty()) {
        return result;
    }

    const QString url = QString(
        "https://geocoding-api.open-meteo.com/v1/search?name=%1&count=%2&language=%3&format=json"
        ).arg(QUrl::toPercentEncoding(query)).arg(count).arg(lang.isEmpty() ? "en" : lang)
    ;

    // process request
    const auto data = m_client.getJson(url);
    if (data.isEmpty()) {
        Logger::warn("OpenMeteoGeocoder: Failed to get search result for <" + query + ">");
        return result;
    }

    const auto doc = QJsonDocument::fromJson(data.toJson());
    if (!doc.isObject()) {
        return result;
    }

    const auto root = doc.object();
    if (!root.contains("results")) {
        return result;
    }

    const auto arr = root["results"].toArray();
    result.reserve(arr.size());

    for (const auto &val : arr) {
        const auto obj = val.toObject();
        Location location;
        location.name = obj.value("name").toString();
        location.latitude = obj.value("latitude").toDouble();
        location.longitude = obj.value("longitude").toDouble();
        location.timezone = obj.value("timezone").toString();
        location.country = obj.value("country").toString();
        location.region = QString("[%1, %2, %3, %4]").
            arg(obj.value("admin1").toString()).
            arg(obj.value("admin2").toString()).
            arg(obj.value("admin3").toString()).
            arg(obj.value("admin4").toString())
        ;

        result.push_back(location);
    }

    return result;
}

std::vector<Location> OpenMeteoGeocoder::search(const QString &text) {
    return search(text, "en", 10);
}
