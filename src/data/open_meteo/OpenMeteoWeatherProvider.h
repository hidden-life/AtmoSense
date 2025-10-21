#ifndef DATA_OPEN_METEO_OPEN_METEO_WEATHER_PROVIDER_H
#define DATA_OPEN_METEO_OPEN_METEO_WEATHER_PROVIDER_H

#include "interfaces/IWeatherProvider.h"
#include "NetworkClient.h"

class OpenMeteoWeatherProvider : public  IWeatherProvider {
    NetworkClient &m_client;
public:
    explicit OpenMeteoWeatherProvider(NetworkClient &client);
    Forecast fetch(double lat, double lon, const QString &tz) override;
    QString name() const override;
};

#endif //DATA_OPEN_METEO_OPEN_METEO_WEATHER_PROVIDER_H
