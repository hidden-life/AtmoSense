#ifndef DOMAIN_INTERFACES_IWEATHER_PROVIDER_H
#define DOMAIN_INTERFACES_IWEATHER_PROVIDER_H

#include "NetworkClient.h"
#include "model/Forecast.h"

class IWeatherProvider {
public:
    virtual ~IWeatherProvider() = default;
    virtual QString id() const = 0;
    virtual QString name() const = 0;
    virtual Forecast fetch(double lat, double lon, const QString &tz) = 0;
    virtual bool apiKeyRequired() { return false; }
};

#endif //DOMAIN_INTERFACES_IWEATHER_PROVIDER_H
