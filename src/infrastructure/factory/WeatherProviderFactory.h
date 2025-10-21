#ifndef INFRASTRUCTURE_FACTORY_WEATHER_PROVIDER_FACTORY_H
#define INFRASTRUCTURE_FACTORY_WEATHER_PROVIDER_FACTORY_H

#include <memory>

#include "NetworkClient.h"
#include "interfaces/IWeatherProvider.h"
#include "model/Provider.h"

class WeatherProviderFactory final {
public:
    static std::shared_ptr<IWeatherProvider> create(const QString &name, NetworkClient &client);
    static std::map<QString, std::shared_ptr<IWeatherProvider>> createAll(NetworkClient &client);
};

#endif //INFRASTRUCTURE_FACTORY_WEATHER_PROVIDER_FACTORY_H
