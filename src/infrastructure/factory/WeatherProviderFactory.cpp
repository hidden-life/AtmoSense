#include "WeatherProviderFactory.h"

#include "open_meteo/OpenMeteoWeatherProvider.h"

std::shared_ptr<IWeatherProvider> WeatherProviderFactory::create(const QString &name, NetworkClient &client) {
    if (name == "OpenMeteo" || name == "Open-Meteo") {
        return std::make_shared<OpenMeteoWeatherProvider>(client);
    }

    throw std::runtime_error(("Unknown weather provider: " + name).toStdString());
}

std::map<QString, std::shared_ptr<IWeatherProvider>> WeatherProviderFactory::createAll(NetworkClient &client) {
    std::map<QString, std::shared_ptr<IWeatherProvider>> providers;

    providers.emplace("OpenMeteo", std::make_shared<OpenMeteoWeatherProvider>(client));

    return providers;
}
