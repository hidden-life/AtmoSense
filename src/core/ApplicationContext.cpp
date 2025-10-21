#include <QStandardPaths>

#include "ApplicationContext.h"
#include "NetworkClient.h"
#include "SettingsManager.h"
#include "ThemeManager.h"
#include "TranslationManager.h"
#include "application/IconMapper.h"
#include "application/UpdateScheduler.h"
#include "cache/FileCacheStore.h"
#include "factory/GeocoderFactory.h"
#include "factory/WeatherProviderFactory.h"
#include "open_meteo/OpenMeteoWeatherProvider.h"
#include "open_meteo/OpenMeteoGeocoder.h"
#include "repository/WeatherRepository.h"

ApplicationContext::ApplicationContext() = default;

int ApplicationContext::weatherProviderCount() const {
    return m_weatherProviders.size();
}

int ApplicationContext::geocoderProviderCount() const {
    return m_geocoderProviders.size();
}

QDir ApplicationContext::cacheDir() const {
    const QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    return dir;
}

void ApplicationContext::rebuildWeatherProvider() {
    m_weatherProvider = WeatherProviderFactory::create("OpenMeteo", *m_networkClient);
}

void ApplicationContext::rebuildGeocoder() {
    m_geocoder = GeocoderFactory::create("OpenMeteo", *m_networkClient);
}

void ApplicationContext::init() {
    m_networkClient = std::make_shared<NetworkClient>();
    m_settings = std::make_shared<SettingsManager>();
    m_cache = std::make_shared<FileCacheStore>(cacheDir());

    // first creation based on settings
    rebuildWeatherProvider();
    rebuildGeocoder();

    m_weatherRepository = std::make_shared<WeatherRepository>(m_weatherProvider, *m_cache);

    // open meteo as default
    m_weatherProvider = std::make_shared<OpenMeteoWeatherProvider>(*m_networkClient);
    m_geocoder = std::make_shared<OpenMeteoGeocoder>(*m_networkClient);


    m_themeManager = std::make_shared<ThemeManager>(m_settings);
    m_updateScheduler = std::make_shared<UpdateScheduler>(m_settings);

    m_translationManager = std::make_shared<TranslationManager>(m_settings);
    m_translationManager->apply();

    m_iconMapper = std::make_shared<IconMapper>();

    connect(m_settings.get(), &SettingsManager::weatherProviderChanged, [this](WeatherProviderId id) {
        rebuildWeatherProvider();
        // "send" new provider to repository
        if (m_weatherProvider) m_weatherRepository->setProvider(m_weatherProvider);

        emit providersChanged();
    });

    connect(m_settings.get(), &SettingsManager::geocoderProviderChanged, [this](GeocoderProviderId) {
        rebuildGeocoder();
        emit providersChanged();
    });
}
