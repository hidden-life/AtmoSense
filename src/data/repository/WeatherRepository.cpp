#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "WeatherRepository.h"

#include "Logger.h"

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

Forecast WeatherRepository::get(const double lat, const double lon, const QString &tz, const int maxAge) {
    const QString key = cacheKey(lat, lon, tz);
    if (const auto cached = m_cacheStore.get(key)) {
        const QJsonDocument doc = QJsonDocument::fromJson(*cached);
        if (!doc.isObject()) {
            Logger::warn("Corrupted data for key <" + key + ">, ignoring.");
        } else {
            const auto root = doc.object();
            Forecast forecast;
            forecast.weather = parse(root.value("now").toObject());
            for (const auto &val : root.value("hourly").toArray()) {
                forecast.hourly.push_back(parse(val.toObject()));
            }

            for (const auto &val : root.value("daily").toArray()) {
                forecast.daily.push_back(parse(val.toObject()));
            }

            Logger::info("Cache git for forecast: <" + key + ">.");
            return forecast;
        }
    }

    // if there is no cache, make call using network
    Logger::info("Cache miss for forecast, fetching using API: <" + key + ">.");
    Forecast forecast;
    try {
        forecast = m_provider.fetch(lat, lon, tz);
    } catch (std::exception &e) {
        Logger::error("Weather provider fetch failed: " + QString::fromStdString(e.what()));
        // fallback and return old cache data
        if (const auto cached = m_cacheStore.get(key)) {
            Logger::warn("Returning stale cache due to provider failure.");
            const QJsonDocument doc = QJsonDocument::fromJson(*cached);
            const auto root = doc.object();
            Forecast f;
            f.weather = parse(root.value("now").toObject());
            for (const auto &val : root.value("hourly").toArray()) {
                f.hourly.push_back(parse(val.toObject()));
            }
            for (const auto &val : root.value("daily").toArray()) {
                f.daily.push_back(parse(val.toObject()));
            }

            return f;
        }
        throw; // if there is no cache, throw!
    }

    // save new answer into the cache
    QJsonObject obj;
    obj["now"] = serialize(forecast.weather);

    QJsonArray hourlyArr;
    for (const auto &val : forecast.hourly) {
        hourlyArr.push_back(serialize(val));
    }
    obj["hourly"] = hourlyArr;

    QJsonArray dailyArr;
    for (const auto &val : forecast.daily) {
        dailyArr.push_back(serialize(val));
    }
    obj["daily"] = dailyArr;

    const QJsonDocument out(obj);
    const QByteArray serialized = out.toJson(QJsonDocument::Compact);

    // TTL in seconds
    const int ttlInSeconds = maxAge * 60;
    m_cacheStore.put(key, serialized, ttlInSeconds);

    Logger::info("WeatherRepository: cached new forecast (" + QString::number(serialized.size()) + ")");

    return forecast;
}
