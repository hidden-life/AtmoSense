#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "WeatherRepository.h"

static QString cacheKey(double lat, double lon, const QString &tz) {
    return QString("forecast:%1,%2,%3").arg(lat, 0, 'f', 4).arg(lon, 0, 'f', 4).arg(tz);
}

static QJsonObject serialize(const Weather &w) {
    QJsonObject obj;
    obj["t"] = w.temperature;
    obj["ws"] = w.windSpeed;
    obj["h"] = w.humidity;
    obj["p"] = w.pressure;
    obj["c"] = w.weatherCode;
    obj["ts"] = w.timestamp.toString(Qt::ISODate);

    return obj;
}

static Weather parse(const QJsonObject &obj) {
    Weather w;
    w.temperature = obj.value("t").toDouble();
    w.windSpeed = obj.value("ws").toDouble();
    w.humidity = obj.value("h").toDouble();
    w.pressure = obj.value("p").toDouble();
    w.weatherCode = obj.value("c").toInt();
    w.timestamp = QDateTime::fromString(obj.value("ts").toString(), Qt::ISODate);

    return w;
}

WeatherRepository::WeatherRepository(IWeatherProvider &provider, ICacheStore &cacheStore) : m_provider(provider), m_cacheStore(cacheStore) {}

Forecast WeatherRepository::get(double lat, double lon, const QString &tz, int maxAge) {
    const QString key = cacheKey(lat, lon, tz);
    if (auto cached = m_cacheStore.get(key)) {
        QJsonDocument doc = QJsonDocument::fromJson(*cached);
        auto root = doc.object();
        Forecast forecast;
        forecast.weather = parse(root.value("now").toObject());
        for (const auto &val : root.value("hourly").toArray()) {
            forecast.hourly.push_back(parse(val.toObject()));
        }

        for (const auto &val : root.value("daily").toArray()) {
            forecast.daily.push_back(parse(val.toObject()));
        }

        return forecast;
    }

    Forecast f = m_provider.fetch(lat, lon, tz);
    QJsonObject obj;
    obj["now"] = serialize(f.weather);

    QJsonArray hourlyArr;
    for (const auto &val : f.hourly) {
        hourlyArr.push_back(serialize(val));
    }
    obj["hourly"] = hourlyArr;

    QJsonArray dailyArr;
    for (const auto &val : f.daily) {
        dailyArr.push_back(serialize(val));
    }
    obj["daily"] = dailyArr;

    QJsonDocument out(obj);

    m_cacheStore.put(key, out.toJson(QJsonDocument::Compact), maxAge * 60);

    return f;
}
