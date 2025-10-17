#include <QStandardPaths>

#include "ApplicationContext.h"
#include "NetworkClient.h"
#include "SettingsManager.h"
#include "ThemeManager.h"
#include "TranslationManager.h"
#include "application/UpdateScheduler.h"
#include "cache/FileCacheStore.h"
#include "open_meteo/OpenMeteoWeatherProvider.h"
#include "open_meteo/OpenMeteoGeocoder.h"
#include "repository/WeatherRepository.h"

ApplicationContext::ApplicationContext() = default;

QDir ApplicationContext::cacheDir() const {
    auto path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return dir;
}

void ApplicationContext::init() {
    m_networkClient = std::make_shared<NetworkClient>();
    m_settings = std::make_shared<SettingsManager>();
    m_cache = std::make_shared<FileCacheStore>(cacheDir());

    // open meteo as default
    m_weatherProvider = std::make_shared<OpenMeteoWeatherProvider>(*m_networkClient);
    m_geocoder = std::make_shared<OpenMeteoGeocoder>(*m_networkClient);

    m_weatherRepository = std::make_shared<WeatherRepository>(*m_weatherProvider, *m_cache);

    m_themeManager = std::make_shared<ThemeManager>(m_settings);
    m_updateScheduler = std::make_shared<UpdateScheduler>(m_settings);

    m_translationManager = std::make_shared<TranslationManager>(m_settings);
    m_translationManager->apply();
}
