#ifndef DOMAIN_REPOSITORY_WEATHER_REPOSITORY_H
#define DOMAIN_REPOSITORY_WEATHER_REPOSITORY_H

#include "interfaces/IWeatherProvider.h"
#include "model/Forecast.h"

class IWeatherRepository {
public:
    virtual ~IWeatherRepository() = default;
    virtual Forecast get(double lat, double lon, const QString &tz, int maxAge = 10) = 0;
    virtual void setProvider(std::shared_ptr<IWeatherProvider> provider) = 0;
};

#endif //DOMAIN_REPOSITORY_WEATHER_REPOSITORY_H
