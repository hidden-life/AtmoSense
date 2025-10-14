#ifndef DOMAIN_REPOSITORY_WEATHER_REPOSITORY_H
#define DOMAIN_REPOSITORY_WEATHER_REPOSITORY_H

#include "model/Forecast.h"

class IWeatherRepository {
public:
    virtual ~IWeatherRepository() = default;
    virtual Forecast get(double lat, double lon, const QString &tz, int maxAge = 10) = 0;
};

#endif //DOMAIN_REPOSITORY_WEATHER_REPOSITORY_H
