#ifndef DATA_REPOSITORY_WEATHER_REPOSITORY_H
#define DATA_REPOSITORY_WEATHER_REPOSITORY_H

#include <memory>

#include "repository/IWeatherRepository.h"
#include "interfaces/IWeatherProvider.h"
#include "interfaces/ICacheStore.h"

struct CacheMetadata {
    QDateTime timestamp;
    int ttl;
};

class WeatherRepository final : public IWeatherRepository {
    std::shared_ptr<IWeatherProvider> m_provider;
    ICacheStore &m_cacheStore;
public:
    WeatherRepository(std::shared_ptr<IWeatherProvider> provider, ICacheStore &cacheStore);
    Forecast get(double lat, double lon, const QString &tz, int maxAge) override;
    void setProvider(std::shared_ptr<IWeatherProvider> provider) override;

private:
    static bool expired(const CacheMetadata &cache);
    static CacheMetadata parseCacheMetadata(const QJsonObject &json);
    static void appendCacheMetadata(QJsonObject &json, int ttl);
};

#endif //DATA_REPOSITORY_WEATHER_REPOSITORY_H
