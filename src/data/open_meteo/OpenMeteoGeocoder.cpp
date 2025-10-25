#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include  <QUrl>

#include "OpenMeteoGeocoder.h"

#include "Logger.h"

OpenMeteoGeocoder::OpenMeteoGeocoder(NetworkClient &client) : m_client(client) {}

std::vector<Location> OpenMeteoGeocoder::search(const QString &query, const QString &lang, const int count) {
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
        Logger::warning("OpenMeteoGeocoder: Failed to get search result for <" + query + ">");
        return result;
    }

    const auto doc = QJsonDocument::fromJson(data.toJson());
    if (!doc.isObject()) {
        Logger::warning("OpenMeteoGeocoder: Failed to parse search result.");
        return result;
    }

    const auto root = doc.object();
    if (!root.contains("results")) {
        Logger::warning("OpenMeteoGeocoder: No results for query <" + query + ">");
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
        // create region string
        QStringList regionParts;
        for (const QString &adminKey : {"admin1", "admin2", "admin3", "admin4"}) {
            const QString v = obj.value(adminKey).toString().trimmed();
            if (!v.isEmpty() && !regionParts.contains(v)) {
                regionParts.append(v);
            }
        }

        // readable region section
        QString full;
        if (!regionParts.isEmpty()) {
            full = regionParts.join(", ");
        }

        if (!location.country.isEmpty()) {
            if (!full.isEmpty()) {
                full += ", ";
            }
            full += location.country;
        }
        location.region = full;

        // filter for duplicates
        auto exists = std::any_of(result.begin(), result.end(), [&](const Location &loc) {
            return qFuzzyCompare(loc.latitude, location.latitude) && qFuzzyCompare(loc.longitude, location.longitude);
        });

        if (!exists) {
            result.push_back(location);
        }
    }

    return result;
}

std::vector<Location> OpenMeteoGeocoder::search(const QString &text, const int count) {
    return search(text, QLocale().name().split('_').first(), count);
}

QString OpenMeteoGeocoder::name() const {
    return "Open-Meteo Geocoder";
}