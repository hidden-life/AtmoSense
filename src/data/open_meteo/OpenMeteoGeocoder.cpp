#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>

#include "OpenMeteoGeocoder.h"

OpenMeteoGeocoder::OpenMeteoGeocoder(NetworkClient &client) : m_client(client) {}

std::vector<Location> OpenMeteoGeocoder::search(const QString &query, const QString &lang, int count) {
    QUrl url("https://geocoding-api.open-meteo.com/v1/search");
    QUrlQuery q;
    q.addQueryItem("name", query);
    q.addQueryItem("count", QString::number(count));
    q.addQueryItem("lang", lang);
    url.setQuery(q);

    QJsonDocument doc = m_client.getJson(url);
    std::vector<Location> result;
    auto obj = doc.object();
    auto resultsArr = obj.value("results").toArray();
    result.reserve(resultsArr.size());

    for (const auto &v : resultsArr) {
        auto o = v.toObject();
        Location location;
        location.name = o.value("name").toString();
        location.country = o.value("country").toString();
        location.region = o.value("admin1").toString();
        location.timezone = o.value("timezone").toString();
        location.latitude = o.value("latitude").toDouble();
        location.longitude = o.value("longitude").toDouble();

        result.push_back(std::move(location));
    }

    return result;
}
