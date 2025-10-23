#include "WeatherProviderFactory.h"

#include "open_meteo/OpenMeteoWeatherProvider.h"

std::shared_ptr<IWeatherProvider> WeatherProviderFactory::create(const QString &name, NetworkClient &client, SettingsManager &settings) {
    if (name == "OpenMeteo" || name == "Open-Meteo") {
        return std::make_shared<OpenMeteoWeatherProvider>(client, settings);
    }

    throw std::runtime_error(("Unknown weather provider: " + name).toStdString());
}

std::map<QString, std::shared_ptr<IWeatherProvider>> WeatherProviderFactory::createAll(NetworkClient &client, SettingsManager &settings) {
    std::map<QString, std::shared_ptr<IWeatherProvider>> providers;

    auto openMeteo = std::make_shared<OpenMeteoWeatherProvider>(client, settings);

    providers.insert({ openMeteo->name(), openMeteo });

    // add another providers here bellow

    return providers;
}
