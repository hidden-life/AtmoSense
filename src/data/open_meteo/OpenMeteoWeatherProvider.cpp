#include <QUrl>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QtMath>

#include "OpenMeteoWeatherProvider.h"

#include "Logger.h"

OpenMeteoWeatherProvider::OpenMeteoWeatherProvider(NetworkClient &client, SettingsManager &settings) : m_client(client), m_settings(settings) {}

Forecast OpenMeteoWeatherProvider::fetch(double lat, double lon, const QString &tz) {
    QUrl url("https://api.open-meteo.com/v1/forecast");
    QUrlQuery query;
    query.addQueryItem("latitude", QString::number(lat));
    query.addQueryItem("longitude", QString::number(lon));
    query.addQueryItem("daily", "weathercode,temperature_2m_max,temperature_2m_min");
    // form hourly query
    QStringList hourlyList = {
        "temperature_2m",
        "relative_humidity_2m",
        "pressure_msl",
        "wind_speed_10m",
        "weathercode",
    };

    if (m_settings.fetchPrecipitationProbability()) {
        hourlyList << "precipitation_probability";
    }
    hourlyList << "precipitation";

    if (m_settings.fetchUV()) {
        hourlyList << "uv_index";
    }
    query.addQueryItem("hourly", hourlyList.join(","));

    // current
    QStringList currentList = {
        "temperature_2m",
        "relative_humidity_2m",
        "pressure_msl",
        "wind_speed_10m",
        "weathercode",
        "precipitation",
    };
    if (m_settings.fetchPrecipitationProbability()) currentList << "precipitation_probability";
    if (m_settings.fetchUV()) currentList << "uv_index";
    query.addQueryItem("current", currentList.join(","));
    query.addQueryItem("timezone", tz.isEmpty() ? "auto" : tz);
    url.setQuery(query);

    QJsonDocument doc = m_client.getJson(url);
    auto root = doc.object();
    qDebug() << "Root: " << root;
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

        if (curr.contains("precipitation")) {
            forecast.weather.precipitation = curr.value("precipitation").toDouble();
        }
        if (m_settings.fetchPrecipitationProbability() && curr.contains("precipitation_probability")) {
            forecast.weather.precipitationProbability = curr.value("precipitation_probability").toDouble();
        }
        if (m_settings.fetchUV() && curr.contains("uv_index")) {
            forecast.weather.uvIndex = curr.value("uv_index").toDouble();
        }
    }

    // hourly
    const auto hourly = root.value("hourly").toObject();
    const auto times = hourly.value("time").toArray();
    const auto temp = hourly.value("temperature_2m").toArray();
    const auto hum = hourly.value("relative_humidity_2m").toArray();
    const auto pres = hourly.value("pressure_msl").toArray();
    const auto wind = hourly.value("wind_speed_10m").toArray();
    const auto code = hourly.value("weathercode").toArray();

    const auto precip = hourly.value("precipitation").toArray();
    const auto precip_prob = hourly.value("precipitation_probability").toArray();
    const auto uv = hourly.value("uv_index").toArray();

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

        if (i < precip.size()) {
            w.precipitation = precip[i].toDouble();
        }
        if (m_settings.fetchPrecipitationProbability() && i < precip_prob.size()) {
            w.precipitationProbability = precip[i].toDouble();
        }
        if (m_settings.fetchUV() && i < uv.size()) {
            w.uvIndex = uv[i].toDouble();
        }

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

    // air quality query
    if (m_settings.fetchAirQuality()) {
        QUrl qualityUrl("https://air-quality-api.open-meteo.com/v1/air-quality");
        QUrlQuery qualityQuery;
        qualityQuery.addQueryItem("latitude", QString::number(lat));
        qualityQuery.addQueryItem("longitude", QString::number(lon));
        qualityQuery.addQueryItem("hourly", "pm10,pm2_5");
        qualityQuery.addQueryItem("timezone", tz.isEmpty() ? "auto" : tz);
        qualityUrl.setQuery(qualityQuery);

        const QJsonDocument qualityDoc = m_client.getJson(qualityUrl);
        const auto qualityRoot = qualityDoc.object();
        const auto qualityHourly = qualityRoot.value("hourly").toObject();

        const auto qualityTimes = qualityHourly.value("time").toArray();
        const auto pm10 = qualityHourly.value("pm10").toArray();
        const auto pm25 = qualityHourly.value("pm2_5").toArray();

        if (!qualityTimes.isEmpty()) {
            AirQuality airQuality;
            airQuality.timestamp = QDateTime::fromString(qualityTimes[0].toString(), Qt::ISODate);
            if (!pm10.isEmpty()) airQuality.pm10 = pm10[0].toDouble();
            if (!pm25.isEmpty()) airQuality.pm2_5 = pm25[0].toDouble();

            forecast.currentAirQuality = airQuality;
        }
    }

    return forecast;
}

QString OpenMeteoWeatherProvider::id() const {
    return "open-meteo";
}

QString OpenMeteoWeatherProvider::name() const {
    return "Open-Meteo";
}
