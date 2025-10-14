#include <QUrl>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QtMath>

#include "OpenMeteoWeatherProvider.h"

OpenMeteoWeatherProvider::OpenMeteoWeatherProvider(NetworkClient &client) : m_client(client) {}

Forecast OpenMeteoWeatherProvider::fetch(double lat, double lon, const QString &tz) {
    QUrl url("https://api.open-meteo.com/v1/forecast");
    QUrlQuery query;
    query.addQueryItem("latitude", QString::number(lat));
    query.addQueryItem("longitude", QString::number(lon));
    query.addQueryItem("hourly", "temperature_2m,relative_humidity_2m,pressure_msl,wind_speed_10m,weathercode");
    query.addQueryItem("daily", "weathercode,temperature_2m_max,temperature_2m_min");
    query.addQueryItem("current", "true");
    query.addQueryItem("timezone", tz.isEmpty() ? "auto" : tz);
    url.setQuery(query);

    QJsonDocument doc = m_client.getJson(url);
    auto root = doc.object();

    Forecast forecast;
    // current
    if (root.contains("current")) {
        auto curr = root.value("current").toObject();
        forecast.weather.temperature = curr.value("temperature_2m").toDouble();
        forecast.weather.windSpeed = curr.value("wind_speed_10m").toDouble() / 3.6; // km/h => m/s
        forecast.weather.humidity = curr.value("relative_humidity_2m").toDouble();
        forecast.weather.pressure = curr.value("pressure_msl").toDouble();
        forecast.weather.weatherCode = curr.value("weathercode").toInt();
        forecast.weather.timestamp = QDateTime::fromString(curr.value("time").toString(), Qt::ISODate);
    }

    // hourly
    auto hourly = root.value("hourly").toObject();
    auto times = hourly.value("time").toArray();
    auto temp = hourly.value("temperature_2m").toArray();
    auto hum = hourly.value("relative_humidity_2m").toArray();
    auto pres = hourly.value("pressure_msl").toArray();
    auto wind = root.value("wind_speed_10m").toArray();
    auto code = hourly.value("weathercode").toArray();

    int n = std::min({ times.size(), temp.size(), hum.size(), pres.size(), wind.size(), code.size() });
    forecast.hourly.reserve(n);

    for (int i = 0; i < n; i++) {
        Weather w;
        w.timestamp = QDateTime::fromString(times[i].toString(), Qt::ISODate);
        w.temperature = temp[i].toDouble();
        w.humidity = hum[i].toDouble();
        w.pressure = pres[i].toDouble();
        w.windSpeed = wind[i].toDouble() / 3.6;
        w.weatherCode = code[i].toInt();

        forecast.hourly.push_back(std::move(w));
    }

    // daily
    auto daily = root.value("daily").toObject();
    auto tm = daily.value("time").toArray();
    auto tempMax = daily.value("temperature_2m_max").toArray();
    auto tempMin = daily.value("temperature_2m_min").toArray();
    auto wCode = daily.value("weathercode").toArray();

    int dn = std::min({ tm.size(), tempMax.size(), tempMin.size(), wCode.size() });
    forecast.daily.reserve(dn);

    for (int i = 0; i < dn; i++) {
        Weather w;
        w.timestamp = QDateTime::fromString(tm[i].toString(), Qt::ISODate);
        w.temperature = (tempMax[i].toDouble() + tempMin[i].toDouble()) / 2.0;
        w.weatherCode = wCode[i].toInt();

        forecast.daily.push_back(std::move(w));
    }

    return forecast;
}
