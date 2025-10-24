#ifndef DATA_OPEN_METEO_OPEN_METEO_WEATHER_PROVIDER_H
#define DATA_OPEN_METEO_OPEN_METEO_WEATHER_PROVIDER_H

#include "interfaces/IWeatherProvider.h"
#include "NetworkClient.h"
#include "SettingsManager.h"

class OpenMeteoWeatherProvider final : public  IWeatherProvider {
    NetworkClient &m_client;
    SettingsManager &m_settings;
public:
    explicit OpenMeteoWeatherProvider(NetworkClient &client, SettingsManager &settings);
    Forecast fetch(double lat, double lon, const QString &tz) override;

    QString id() const override;
    QString name() const override;

    NetworkClient &client() override { return m_client; }
};

#endif //DATA_OPEN_METEO_OPEN_METEO_WEATHER_PROVIDER_H
