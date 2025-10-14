#ifndef DOMAIN_INTERFACES_IWEATHER_PROVIDER_H
#define DOMAIN_INTERFACES_IWEATHER_PROVIDER_H

#include "model/Forecast.h"

class IWeatherProvider {
public:
    virtual ~IWeatherProvider() = default;
    virtual Forecast fetch(double lat, double lon, const QString &tz) = 0;
};

#endif //DOMAIN_INTERFACES_IWEATHER_PROVIDER_H
