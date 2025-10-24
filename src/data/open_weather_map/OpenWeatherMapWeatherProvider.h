#ifndef DATA_OPEN_WEATHER_MAP_OPEN_WEATHER_MAP_WEATHER_PROVIDER_H
#define DATA_OPEN_WEATHER_MAP_OPEN_WEATHER_MAP_WEATHER_PROVIDER_H

#include <QJsonObject>
#include <QJsonArray>

#include "interfaces/IWeatherProvider.h"

class OpenWeatherMapWeatherProvider final : public IWeatherProvider {
public:
    explicit OpenWeatherMapWeatherProvider(NetworkClient &client, QString apiKey);
    [[nodiscard]]
    QString id() const override { return "open-weather-map"; }
    [[nodiscard]]
    QString name() const override { return "OpenWeatherMap"; };
    Forecast fetch(double lat, double lon, const QString &tz) override;
    bool apiKeyRequired() override;
    NetworkClient &client() override { return m_client; }

private:
    NetworkClient &m_client;
    QString m_apiKey;
};

#endif //DATA_OPEN_WEATHER_MAP_OPEN_WEATHER_MAP_WEATHER_PROVIDER_H
