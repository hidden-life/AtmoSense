#include "WeatherProviderFactory.h"

#include "Logger.h"
#include "open_meteo/OpenMeteoWeatherProvider.h"
#include "open_weather_map/OpenWeatherMapWeatherProvider.h"

std::shared_ptr<IWeatherProvider> WeatherProviderFactory::create(const QString &name, NetworkClient &client, SettingsManager &settings) {
    if (name == "OpenMeteo" || name == "Open-Meteo") {
        return std::make_shared<OpenMeteoWeatherProvider>(client, settings);
    }

    throw std::runtime_error(("Unknown weather provider: " + name).toStdString());
}

std::map<QString, std::shared_ptr<IWeatherProvider>> WeatherProviderFactory::createAll(NetworkClient &client, SettingsManager &settings) {
    std::map<QString, std::shared_ptr<IWeatherProvider>> providers;

    // open meteo
    providers[toString(WeatherProviderId::OpenMeteo)] = std::make_shared<OpenMeteoWeatherProvider>(client, settings);
    // open weather map
    const QString key = settings.openWeatherMapAPIKey().trimmed();
   if (!key.isEmpty()) {
       providers[toString(WeatherProviderId::OpenWeatherMap)] = std::make_shared<OpenWeatherMapWeatherProvider>(client, key);
   } else {
       Logger::warning("OpenWeatherMap API key missing, skipping provider.");
   }

    Logger::info(QString("WeatherProviderFactory: total providers = %1").arg(providers.size()));

    return providers;
}
