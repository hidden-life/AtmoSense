#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "WeatherRepository.h"

#include "Logger.h"

static QString cacheKey(const double lat, const double lon, const QString &tz) {
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

    if (w.precipitation.has_value()) obj["pr"] = *w.precipitation;
    if (w.precipitationProbability.has_value()) obj["pp"] = *w.precipitationProbability;
    if (w.uvIndex.has_value()) obj["uv"] = *w.uvIndex;

    return obj;
}

static QJsonObject serializeAirQuality(const AirQuality &q) {
    QJsonObject obj;
    obj["ts"] = q.timestamp.toString(Qt::ISODate);
    if (q.pm10.has_value()) obj["pm10"] = *q.pm10;
    if (q.pm2_5.has_value()) obj["pm25"] = *q.pm2_5;

    return obj;
}

static AirQuality parseAirQuality(const QJsonObject &obj) {
    AirQuality airQuality;
    airQuality.timestamp = QDateTime::fromString(obj.value("ts").toString(), Qt::ISODate);
    if (obj.contains("pm10")) airQuality.pm10 = obj.value("pm10").toDouble();
    if (obj.contains("pm25")) airQuality.pm2_5 = obj.value("pm25").toDouble();

    return airQuality;
}

static Weather parse(const QJsonObject &obj) {
    Weather w;
    w.temperature = obj.value("t").toDouble();
    w.windSpeed = obj.value("ws").toDouble();
    w.humidity = obj.value("h").toDouble();
    w.pressure = obj.value("p").toDouble();
    w.weatherCode = obj.value("c").toInt();
    w.timestamp = QDateTime::fromString(obj.value("ts").toString(), Qt::ISODate);

    if (obj.contains("pr")) w.precipitation = obj.value("pr").toDouble();
    if (obj.contains("pp")) w.precipitationProbability = obj.value("pp").toDouble();
    if (obj.contains("uv")) w.uvIndex = obj.value("uv").toDouble();

    return w;
}

WeatherRepository::WeatherRepository(std::shared_ptr<IWeatherProvider> provider, ICacheStore &cacheStore) :
    m_provider(std::move(provider)), m_cacheStore(cacheStore) {}

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

            if (root.contains("aq") && root.value("aq").isObject()) {
                forecast.currentAirQuality = parseAirQuality(root.value("aq").toObject());
            }

            Logger::info("Cache hit for forecast: <" + key + ">.");
            return forecast;
        }
    }

    // if there is no cache, make call using network
    Logger::info("Cache miss for forecast, fetching using API: <" + key + ">.");
    Forecast forecast;
    try {
        if (!m_provider) {
            Logger::error("Weather provider is not initialized.");
            throw std::runtime_error("Weather provider missing.");
        }

        forecast = m_provider->fetch(lat, lon, tz);
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

    if (forecast.currentAirQuality.has_value()) {
        obj["aq"] = serializeAirQuality(*forecast.currentAirQuality);
    }

    const QJsonDocument out(obj);
    const QByteArray serialized = out.toJson(QJsonDocument::Compact);

    // TTL in seconds
    const int ttlInSeconds = maxAge * 60;
    m_cacheStore.put(key, serialized, ttlInSeconds);

    Logger::info("WeatherRepository: cached new forecast (" + QString::number(serialized.size()) + ")");

    return forecast;
}

void WeatherRepository::setProvider(std::shared_ptr<IWeatherProvider> provider) {
    m_provider = std::move(provider);
}
