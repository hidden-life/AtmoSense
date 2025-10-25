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
    const int ttl = 24 * 3600;

    if (const auto cached = m_cacheStore.get(key)) {
        m_lastFromCache = true;
        if (const QJsonDocument doc = QJsonDocument::fromJson(*cached); doc.isObject()) {
            const auto root = doc.object();
            const CacheMetadata cacheMetadata = parseCacheMetadata(root);
            const bool isExpired = expired(cacheMetadata);

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

            if (!isExpired) {
                Logger::info("WeatherRepository: using cache forecast for <" + key + ">");
                return forecast;
            }

            Logger::warning("WeatherRepository: cache expired for <" + key + ">");
        }
    }

    // request freq
    static QDateTime lastRequest;
    if (const int minInterval = maxAge * 60; lastRequest.isValid() && lastRequest.secsTo(QDateTime::currentDateTimeUtc()) < minInterval) {
        Logger::warning("WeatherRepository: skipping API call (rate limit).");
        if (const auto cached = m_cacheStore.get(key)) {
            if (const QJsonDocument doc = QJsonDocument::fromJson(*cached); doc.isObject()) {
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
                m_lastFromCache = true;

                return forecast;
            }
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

        if (!m_provider->client().hasInternet()) {
            Logger::warning("No internet connection. Loading from cache.");
            if (const auto cached = m_cacheStore.get(key)) {
                Logger::info("Using cached forecast due to no internet.");
                const QJsonDocument doc = QJsonDocument::fromJson(*cached);
                if (doc.isObject()) {
                    const auto root = doc.object();
                    Forecast f;
                    f.weather = parse(root.value("now").toObject());
                    for (const auto &val : root.value("hourly").toArray()) {
                        f.hourly.push_back(parse(val.toObject()));
                    }
                    for (const auto &val : root.value("daily").toArray()) {
                        f.daily.push_back(parse(val.toObject()));
                    }

                    if (root.contains("aq") && root.value("aq").isObject()) {
                        f.currentAirQuality = parseAirQuality(root.value("aq").toObject());
                    }
                    m_lastFromCache = true;

                    return f;
                }
            }

            throw std::runtime_error("No internet connection and no cached data available.");
        }

        m_lastFromCache = false;
        forecast = m_provider->fetch(lat, lon, tz);
        lastRequest = QDateTime::currentDateTimeUtc();
    } catch (std::exception &e) {
        Logger::error("Weather provider fetch failed: " + QString::fromStdString(e.what()));
        // fallback and return old cache data
        if (const auto cached = m_cacheStore.get(key)) {
            Logger::warning("Returning stale cache due to provider failure.");
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
            m_lastFromCache = true;

            return f;
        }

        m_lastFromCache = false;
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

    m_lastUpdated = QDateTime::currentDateTimeUtc();
    appendCacheMetadata(obj, ttl);
    const QJsonDocument out(obj);
    const QByteArray serialized = out.toJson(QJsonDocument::Compact);
    m_cacheStore.put(key, serialized, ttl);

    Logger::info("WeatherRepository: cached new forecast (" + QString::number(serialized.size()) + ")");

    return forecast;
}

void WeatherRepository::setProvider(std::shared_ptr<IWeatherProvider> provider) {
    m_provider = std::move(provider);
}

bool WeatherRepository::expired(const CacheMetadata &cache) {
    if (!cache.timestamp.isValid()) return true;

    return cache.timestamp.secsTo(QDateTime::currentDateTimeUtc()) > cache.ttl;
}

CacheMetadata WeatherRepository::parseCacheMetadata(const QJsonObject &json) {
    CacheMetadata cache;
    cache.timestamp = QDateTime::fromString(json.value("__cached_at").toString(), Qt::ISODate);
    cache.ttl = json.value("__ttl").toInt(24 * 3600);

    return cache;
}

void WeatherRepository::appendCacheMetadata(QJsonObject &json, const int ttl) {
    json["__ttl"] = ttl;
    json["__cached_at"] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
}
