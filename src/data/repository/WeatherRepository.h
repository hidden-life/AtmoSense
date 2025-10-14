#ifndef DATA_REPOSITORY_WEATHER_REPOSITORY_H
#define DATA_REPOSITORY_WEATHER_REPOSITORY_H

#include <memory>

#include "repository/IWeatherRepository.h"
#include "interfaces/IWeatherProvider.h"
#include "interfaces/ICacheStore.h"

class WeatherRepository : public IWeatherRepository {
    IWeatherProvider &m_provider;
    ICacheStore &m_cacheStore;
public:
    WeatherRepository(IWeatherProvider &provider, ICacheStore &cacheStore);
    Forecast get(double lat, double lon, const QString &tz, int maxAge) override;
};

#endif //DATA_REPOSITORY_WEATHER_REPOSITORY_H
